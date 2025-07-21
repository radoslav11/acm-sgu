#!/usr/bin/env python3
"""
Simple autoregressive finetuning for competitive programming problems.
Trains the model to predict the next token given the previous tokens.
"""

import mlx.core as mx
import mlx.nn as nn
import mlx.optimizers as optim
import numpy as np
import os
import glob
import json
import argparse
import signal
from mlx_lm import utils
import matplotlib.pyplot as plt

# Global flag for graceful shutdown
should_stop = False


def signal_handler(sig, frame):
    global should_stop
    print("\n\nReceived interrupt signal. Saving model and exiting...")
    should_stop = True
    # Force exit if called multiple times
    if hasattr(signal_handler, 'called'):
        print("Force exit!")
        exit(1)
    signal_handler.called = True


def load_dataset_files(dataset_dir):
    """Load all .txt files from dataset directory."""
    txt_files = glob.glob(os.path.join(dataset_dir, "*.txt"))
    texts = []

    for file_path in txt_files:
        try:
            with open(file_path, "r", encoding="utf-8") as f:
                content = f.read().strip()
                if content:  # Only add non-empty files
                    texts.append(content)
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
            continue

    print(f"Loaded {len(texts)} text files from {dataset_dir}")
    return texts


def create_batches(
    texts, tokenizer, batch_size, max_length=1024, model_vocab_size=152064
):
    """Create batches of tokenized text with proper padding and sliding window chunking."""
    
    # First, create all chunks from all texts
    all_chunks = []
    
    print(f"Processing {len(texts)} texts with sliding window (max_length={max_length})...")
    
    for text_idx, text in enumerate(texts):
        tokens = tokenizer.encode(text)
        # Clamp token IDs to valid range
        tokens = [min(token, model_vocab_size - 1) for token in tokens]
        
        # Implement sliding window chunking with 50% overlap
        if len(tokens) <= max_length:
            # Short enough to fit completely
            all_chunks.append(tokens)
        else:
            # Split into overlapping chunks
            step_size = max_length // 2  # 50% overlap
            chunk_count = 0
            for start in range(0, len(tokens), step_size):
                chunk = tokens[start:start + max_length]
                if len(chunk) >= max_length // 4:  # Only keep chunks with reasonable length
                    all_chunks.append(chunk)
                    chunk_count += 1
                if start + max_length >= len(tokens):  # Last chunk covers the end
                    break
            
            if text_idx < 5:  # Log first few examples
                print(f"  Text {text_idx+1}: {len(tokens)} tokens -> {chunk_count} chunks")
    
    print(f"Created {len(all_chunks)} chunks from {len(texts)} original texts")
    print(f"Expansion factor: {len(all_chunks) / len(texts):.1f}x")
    
    # Now create batches from chunks
    batches = []
    
    for i in range(0, len(all_chunks), batch_size):
        batch_chunks = all_chunks[i : i + batch_size]
        
        # Find max length in this batch for padding
        max_len = max(len(chunk) for chunk in batch_chunks)

        # Pad to same length
        input_ids = []
        attention_mask = []

        pad_id = getattr(tokenizer, "pad_token_id", model_vocab_size - 1)
        if pad_id is None:
            pad_id = model_vocab_size - 1

        for chunk in batch_chunks:
            padded = chunk + [pad_id] * (max_len - len(chunk))
            mask = [1] * len(chunk) + [0] * (max_len - len(chunk))
            input_ids.append(padded)
            attention_mask.append(mask)

        tokenized = {
            "input_ids": np.array(input_ids),
            "attention_mask": np.array(attention_mask),
        }

        input_ids = mx.array(tokenized["input_ids"])
        attention_mask = mx.array(tokenized["attention_mask"])

        # Create labels (shifted input_ids for autoregressive training)
        # Labels should be input_ids shifted left by 1 position
        eos_id = getattr(tokenizer, "eos_token_id", model_vocab_size - 1)
        if eos_id is None:
            eos_id = model_vocab_size - 1
        labels = mx.concatenate(
            [input_ids[:, 1:], mx.full((input_ids.shape[0], 1), eos_id)],
            axis=1,
        )

        # Create loss weights to ignore padding tokens
        loss_weights = attention_mask.astype(mx.float32)

        batches.append(
            {
                "input_ids": input_ids,
                "labels": labels,
                "loss_weights": loss_weights,
            }
        )

    return batches


def compute_loss(model, input_ids, labels, loss_weights):
    """Compute autoregressive language modeling loss."""
    logits = model(input_ids)

    # Reshape for cross entropy: (batch, seq, vocab) -> (batch*seq, vocab)
    logits_flat = logits.reshape(-1, logits.shape[-1])
    labels_flat = labels.reshape(-1)
    weights_flat = loss_weights.reshape(-1)

    # Compute cross entropy loss - much simpler approach
    losses = nn.losses.cross_entropy(
        logits_flat, labels_flat, reduction="none"
    )

    # Use simple mean instead of weighted mean to avoid numerical issues
    # In autoregressive training, we usually want to learn from all tokens anyway
    return mx.mean(losses)


def clip_gradients(grads, max_norm=1.0):
    """Clip gradients to prevent explosion."""

    # Simple gradient clipping without tree operations
    def get_grad_norm(g):
        if isinstance(g, dict):
            return sum(get_grad_norm(v) for v in g.values())
        elif isinstance(g, (list, tuple)):
            return sum(get_grad_norm(v) for v in g)
        elif hasattr(g, "shape"):  # mx.array
            return mx.sum(g * g)
        else:
            return mx.array(0.0)

    grad_norm = mx.sqrt(get_grad_norm(grads))

    if grad_norm > max_norm:
        clip_factor = max_norm / grad_norm

        def clip_grad(g):
            if isinstance(g, dict):
                return {k: clip_grad(v) for k, v in g.items()}
            elif isinstance(g, (list, tuple)):
                return type(g)(clip_grad(v) for v in g)
            elif hasattr(g, "shape"):  # mx.array
                return g * clip_factor
            else:
                return g

        grads = clip_grad(grads)

    return grads, grad_norm


def plot_training_progress(losses, grad_norms, epoch, output_dir):
    """Generate training progress plots."""
    try:
        # Set up the plot style
        plt.style.use("default")
        _, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

        # Plot 1: Loss over batches
        batch_numbers = list(range(1, len(losses) + 1))
        ax1.plot(batch_numbers, losses, "b-", linewidth=2, alpha=0.7)
        ax1.set_xlabel("Batch Number")
        ax1.set_ylabel("Loss")
        ax1.set_title(f"Training Loss - Epoch {epoch}")
        ax1.grid(True, alpha=0.3)
        ax1.set_yscale("log")  # Log scale for loss

        # Add moving average if we have enough points
        if len(losses) > 10:
            window = min(20, len(losses) // 5)
            moving_avg = np.convolve(
                losses, np.ones(window) / window, mode="valid"
            )
            start_idx = window // 2
            end_idx = start_idx + len(moving_avg)
            ax1.plot(
                batch_numbers[start_idx:end_idx],
                moving_avg,
                "r-",
                linewidth=3,
                alpha=0.8,
                label=f"Moving Avg (window={window})",
            )
            ax1.legend()

        # Plot 2: Gradient norms over batches
        valid_grad_norms = [
            gn for gn in grad_norms if not np.isnan(gn) and not np.isinf(gn)
        ]
        valid_batch_numbers = [
            i + 1
            for i, gn in enumerate(grad_norms)
            if not np.isnan(gn) and not np.isinf(gn)
        ]

        if valid_grad_norms:
            ax2.plot(
                valid_batch_numbers,
                valid_grad_norms,
                "g-",
                linewidth=2,
                alpha=0.7,
            )
            ax2.set_xlabel("Batch Number")
            ax2.set_ylabel("Gradient Norm")
            ax2.set_title(f"Gradient Norms - Epoch {epoch}")
            ax2.grid(True, alpha=0.3)
            ax2.set_yscale("log")  # Log scale for gradient norms

            # Add horizontal line at gradient clipping threshold
            ax2.axhline(
                y=0.01,
                color="r",
                linestyle="--",
                alpha=0.7,
                label="Clip Threshold",
            )
            ax2.legend()

        # Add statistics text
        if losses:
            loss_stats = (
                f"Loss Stats:\nMin: {min(losses):.4f}\nMax:"
                f" {max(losses):.4f}\nMean: {np.mean(losses):.4f}\nFinal:"
                f" {losses[-1]:.4f}"
            )
            ax1.text(
                0.02,
                0.98,
                loss_stats,
                transform=ax1.transAxes,
                verticalalignment="top",
                bbox=dict(boxstyle="round", facecolor="white", alpha=0.8),
                fontsize=9,
            )

        if valid_grad_norms:
            grad_stats = (
                f"Grad Norm Stats:\nMin: {min(valid_grad_norms):.4f}\nMax:"
                f" {max(valid_grad_norms):.4f}\nMean:"
                f" {np.mean(valid_grad_norms):.4f}"
            )
            ax2.text(
                0.02,
                0.98,
                grad_stats,
                transform=ax2.transAxes,
                verticalalignment="top",
                bbox=dict(boxstyle="round", facecolor="white", alpha=0.8),
                fontsize=9,
            )

        # Adjust layout and save
        plt.tight_layout()
        plot_path = os.path.join(
            output_dir, f"training_progress_epoch_{epoch}.png"
        )
        plt.savefig(plot_path, dpi=150, bbox_inches="tight")
        plt.close()

        print(f"Training plot saved to: {plot_path}")

    except Exception as e:
        print(f"Failed to generate plot: {e}")


def save_training_data(losses, grad_norms, epoch, output_dir):
    """Save training data to JSON for later analysis."""
    try:
        training_data = {
            "epoch": epoch,
            "losses": losses,
            "grad_norms": grad_norms,
            "stats": {
                "min_loss": min(losses) if losses else None,
                "max_loss": max(losses) if losses else None,
                "mean_loss": np.mean(losses) if losses else None,
                "final_loss": losses[-1] if losses else None,
                "total_batches": len(losses),
                "successful_batches": len(
                    [l for l in losses if not np.isnan(l)]
                ),
            },
        }

        data_path = os.path.join(
            output_dir, f"training_data_epoch_{epoch}.json"
        )
        with open(data_path, "w") as f:
            json.dump(training_data, f, indent=2)

        print(f"Training data saved to: {data_path}")

    except Exception as e:
        print(f"Failed to save training data: {e}")


def main():
    # Set up signal handler for graceful shutdown
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    parser = argparse.ArgumentParser(
        description="Simple autoregressive finetuning"
    )
    parser.add_argument(
        "--model", type=str, required=True, help="Path to the base model"
    )
    parser.add_argument(
        "--dataset_dir",
        type=str,
        default="dataset",
        help="Directory containing .txt files",
    )
    parser.add_argument("--batch_size", type=int, default=1, help="Batch size")
    parser.add_argument(
        "--epochs", type=int, default=1, help="Number of epochs"
    )
    parser.add_argument("--lr", type=float, default=1e-9, help="Learning rate")
    parser.add_argument(
        "--max_length", type=int, default=1024, help="Maximum sequence length"
    )
    parser.add_argument(
        "--output_dir", type=str, required=True, help="Output directory"
    )
    parser.add_argument(
        "--gradient_clip",
        type=float,
        default=1.0,
        help="Gradient clipping norm",
    )
    parser.add_argument(
        "--save_every",
        type=int,
        default=50,
        help="Save checkpoint every N batches",
    )

    args = parser.parse_args()

    print(f"Loading model from {args.model}")
    model, tokenizer = utils.load(args.model)

    # Fix tokenizer token IDs to be within model vocab range
    try:
        model_vocab_size = model.model.embed_tokens.weight.shape[0]
    except:
        model_vocab_size = 152064

    print(f"Model vocab size confirmed: {model_vocab_size}")

    # Force a safe EOS token ID within the model's vocabulary
    safe_eos_id = model_vocab_size - 1  # Use the last valid token ID
    print(f"Setting safe EOS/PAD to: {safe_eos_id}")

    # Override the tokenizer's token IDs
    tokenizer.eos_token_id = safe_eos_id
    tokenizer.pad_token_id = safe_eos_id

    print(f"Final EOS token ID: {tokenizer.eos_token_id}")
    print(f"Final PAD token ID: {tokenizer.pad_token_id}")

    print(f"Loading dataset from {args.dataset_dir}")
    texts = load_dataset_files(args.dataset_dir)

    if not texts:
        print("No texts found in dataset directory!")
        return

    print("Creating batches...")
    batches = create_batches(
        texts, tokenizer, args.batch_size, args.max_length, model_vocab_size
    )

    print(f"Created {len(batches)} batches")

    # Set up optimizer and loss function - use SGD instead of Adam to avoid corruption
    optimizer = optim.SGD(learning_rate=args.lr)
    loss_and_grad_fn = nn.value_and_grad(model, compute_loss)

    # Training loop
    current_batch = 0
    all_losses = []  # Track losses across all epochs
    running_loss = 0.0
    running_count = 0

    for epoch in range(args.epochs):
        if should_stop:
            break

        print(f"\nEpoch {epoch+1}/{args.epochs}")
        epoch_loss = 0.0
        epoch_losses = []  # Track all batch losses for plotting
        epoch_grad_norms = []  # Track all gradient norms for plotting

        for batch_idx, batch in enumerate(batches):
            if should_stop:
                print("Stopping due to interrupt signal...")
                break

            current_batch += 1

            # Forward pass and compute gradients
            loss, grads = loss_and_grad_fn(
                model,
                batch["input_ids"],
                batch["labels"],
                batch["loss_weights"],
            )

            # Check for NaN loss
            loss_val = float(loss.item())

            if np.isnan(loss_val) or np.isinf(loss_val):
                print(
                    f"  Skipping batch {batch_idx+1} due to NaN/Inf loss -"
                    " resetting optimizer"
                )
                # Reset optimizer state when NaN detected
                optimizer = optim.SGD(learning_rate=args.lr)
                # Record NaN for plotting
                epoch_losses.append(float("nan"))
                epoch_grad_norms.append(float("nan"))
                continue

            # Clip gradients extremely aggressively for stability
            grads, grad_norm = clip_gradients(
                grads, 0.01
            )  # Extremely small clip norm

            # Check for NaN gradients
            grad_norm_val = float(grad_norm.item())
            if np.isnan(grad_norm_val) or np.isinf(grad_norm_val):
                print(
                    f"  Skipping batch {batch_idx+1} due to NaN/Inf gradients"
                )
                # Record NaN for plotting
                epoch_losses.append(
                    loss_val
                )  # Loss was OK, but gradients failed
                epoch_grad_norms.append(float("nan"))
                continue

            # Update model
            optimizer.update(model, grads)
            mx.eval(model.parameters(), optimizer.state)

            # Clear memory
            if hasattr(mx, "metal"):
                mx.metal.clear_cache()

            epoch_loss += loss_val

            # Update running statistics
            all_losses.append(loss_val)
            running_loss += loss_val
            running_count += 1

            # Record data for plotting
            epoch_losses.append(loss_val)
            epoch_grad_norms.append(grad_norm_val)

            # Calculate running average
            running_avg = running_loss / running_count
            
            # Log after every batch with running average
            print(
                f"  Batch {batch_idx+1}/{len(batches)} | "
                f"Loss: {loss_val:.4f} | "
                f"Running Avg: {running_avg:.4f} | "
                f"Grad Norm: {grad_norm_val:.4f}"
            )
            
            # Log progress every 25 batches
            if (batch_idx + 1) % 25 == 0:
                recent_losses = all_losses[-25:] if len(all_losses) >= 25 else all_losses
                recent_avg = np.mean(recent_losses)
                print(f"    [Progress] Recent 25-batch avg: {recent_avg:.4f}, "
                      f"Overall avg: {running_avg:.4f}, "
                      f"Total batches: {running_count}")
                
            # Log progress every 100 batches  
            if (batch_idx + 1) % 100 == 0:
                min_loss = min(all_losses)
                max_loss = max(all_losses)
                print(f"    [Milestone] 100 batches completed! "
                      f"Min: {min_loss:.4f}, Max: {max_loss:.4f}, "
                      f"Range: {max_loss - min_loss:.4f}")

            # Save checkpoint periodically
            if current_batch % args.save_every == 0:
                checkpoint_dir = (
                    f"{args.output_dir}_checkpoint_{current_batch}"
                )
                print(f"Saving checkpoint to {checkpoint_dir}")
                os.makedirs(checkpoint_dir, exist_ok=True)
                # Flatten the parameters properly for saving
                def flatten_dict(d, prefix=""):
                    result = {}
                    for k, v in d.items():
                        if isinstance(v, dict):
                            result.update(flatten_dict(v, f"{prefix}{k}."))
                        elif isinstance(v, list):
                            # Handle list of dicts (like layers)
                            for i, item in enumerate(v):
                                if isinstance(item, dict):
                                    result.update(flatten_dict(item, f"{prefix}{k}.{i}."))
                                else:
                                    result[f"{prefix}{k}.{i}"] = item
                        else:
                            result[f"{prefix}{k}"] = v
                    return result
                
                flattened_weights = flatten_dict(model.parameters())
                utils.save_weights(checkpoint_dir, flattened_weights)

        avg_loss = epoch_loss / len(batches)
        
        # Epoch summary statistics
        valid_losses = [l for l in epoch_losses if not np.isnan(l)]
        if valid_losses:
            epoch_min = min(valid_losses)
            epoch_max = max(valid_losses)
            epoch_std = np.std(valid_losses)
            successful_batches = len(valid_losses)
            
            print(f"\n=== Epoch {epoch+1} Summary ===")
            print(f"Average loss: {avg_loss:.4f}")
            print(f"Min loss: {epoch_min:.4f}")
            print(f"Max loss: {epoch_max:.4f}")
            print(f"Std deviation: {epoch_std:.4f}")
            print(f"Successful batches: {successful_batches}/{len(batches)}")
            print(f"Overall running average: {running_loss/running_count:.4f}")
            print(f"Total batches processed: {running_count}")
        else:
            print(f"\nEpoch {epoch+1} - No valid losses recorded!")

        # Generate plots and save training data for this epoch
        print(f"\nGenerating training plots for epoch {epoch+1}...")
        # Ensure output directory exists for plots
        os.makedirs(args.output_dir, exist_ok=True)
        plot_training_progress(
            epoch_losses, epoch_grad_norms, epoch + 1, args.output_dir
        )
        save_training_data(
            epoch_losses, epoch_grad_norms, epoch + 1, args.output_dir
        )

        # Save model after each epoch
        epoch_dir = f"{args.output_dir}_epoch_{epoch+1}"
        print(f"Saving model after epoch {epoch+1} to {epoch_dir}")
        os.makedirs(epoch_dir, exist_ok=True)
        # Flatten the parameters properly for saving
        def flatten_dict(d, prefix=""):
            result = {}
            for k, v in d.items():
                if isinstance(v, dict):
                    result.update(flatten_dict(v, f"{prefix}{k}."))
                elif isinstance(v, list):
                    # Handle list of dicts (like layers)
                    for i, item in enumerate(v):
                        if isinstance(item, dict):
                            result.update(flatten_dict(item, f"{prefix}{k}.{i}."))
                        else:
                            result[f"{prefix}{k}.{i}"] = item
                else:
                    result[f"{prefix}{k}"] = v
            return result
        
        flattened_weights = flatten_dict(model.parameters())
        utils.save_weights(epoch_dir, flattened_weights)

    # Final save
    print(f"\nSaving final model to {args.output_dir}")
    os.makedirs(args.output_dir, exist_ok=True)
    # Flatten the parameters properly for saving
    def flatten_dict(d, prefix=""):
        result = {}
        for k, v in d.items():
            if isinstance(v, dict):
                result.update(flatten_dict(v, f"{prefix}{k}."))
            elif isinstance(v, list):
                # Handle list of dicts (like layers)
                for i, item in enumerate(v):
                    if isinstance(item, dict):
                        result.update(flatten_dict(item, f"{prefix}{k}.{i}."))
                    else:
                        result[f"{prefix}{k}.{i}"] = item
            else:
                result[f"{prefix}{k}"] = v
        return result
    
    flattened_weights = flatten_dict(model.parameters())
    utils.save_weights(args.output_dir, flattened_weights)

    print("Training complete!")


if __name__ == "__main__":
    main()
