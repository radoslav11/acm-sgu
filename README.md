# ACM SGU Competitive Programming Solutions with LLM Enhancement

This repository contains solutions to ACM SGU (Saratov State University) competitive programming problems, enhanced with detailed editorials and reasoning explanations generated using advanced language models, and includes a finetuned model trained specifically on this dataset.

## Overview

The repository consists of three main components:

1. **Original Solutions**: Competitive programming solutions to SGU problems in C++ or Python.
2. **Enhanced Editorials**: Comprehensive problem explanations, solution approaches, and detailed editorials generated using o4-mini.
3. **Finetuned Model**: A basic pipeline with MLX for finetuning an LLM based on the above (still WIP).

## Dataset Structure

```
dataset/
├── p100.txt          # Enhanced editorial with solution approach
├── p100_raw.txt      # Original source code solution + statemtent + sample input/output
├── p101.txt          # Enhanced editorial with solution approach
├── p101_raw.txt      # Original source code solution + statemtent + sample input/output
└── ...               # Additional problems (100+ problems covered)
```

Each enhanced editorial (`p*.txt`) contains:
- Concise problem statement.
- Detailed solution approach and algorithm explanation.
- Step-by-step implementation guide.
- Time/space complexity analysis.
- Alternative solution methods.
- C++ and Python reference implementations.

## Enhanced Editorial Format

The editorials follow a structured format:
1. **Abridged Problem Statement**: Clear, concise problem description.
2. **Detailed Editorial**: Algorithm explanation, key insights, and approach.
3. **Implementation Details**: Step-by-step coding guidance.
4. **Reference Solutions**: One solution in C++ and one in Python.
5. **Compressed Editorial**: Quick summary for experienced programmers.

## Model Finetuning

### Dataset Preparation
- **181 problems** (and slowly increasing) from the SGU archive solved by me ([radoslav11](https://codeforces.com/problemsets/acmsguru/standings)).
- Enhanced with detailed editorials using GPT o4-mini.

### Training Configuration
- **Base Model**: OlympicCoder-7B (MLX format, fp16)
- **Architecture**: Qwen2-based language model
- **Training Method**: Autoregressive language modeling with SGD optimizer
- **Context Length**: 2048 tokens with sliding window
- **Learning Rate**: 1e-6 with gradient clipping
- **Hardware**: Apple M2 Max (96GB RAM)

## Getting Started

### Prerequisites
```bash
# Install MLX for Apple Silicon
pip install mlx mlx-lm

# Install additional dependencies
pip install matplotlib numpy transformers
```

### Training Your Own Model
```bash
python3 src/finetune.py \
    --model path/to/base/model \
    --dataset_dir dataset \
    --epochs 3 \
    --batch_size 1 \
    --lr 1e-6 \
    --max_length 2048 \
    --output_dir path/to/output
```

## File Structure

```
src/
├── process_problems.py          # Script to process my raw solutions and statments.
├── create_dataset.py            # After processing the problems, creates the actual dataset.
├── finetune.py                  # Main training pipeline script.
└── requirements.txt             # Python dependencies.

problems/
├── p*/
├───── statement.txt            # Original problem statement.
└───── p*.{cpp,py}              # The original solution in C++ or Python.

dataset/
├── p*.txt                      # Enhanced editorials.
├── p*_finetune.tx              # Data used for finetuning. 
└── p*_raw.txt                  # All data from the corresponding promblems/ directory.
```

## External Requirements

The training pipeline requires:
- **Apple Silicon Mac** with MLX support.
- **Sufficient RAM** (recommended 32GB+ for 7B models).
- **Python 3.8+** with MLX framework.
- **Matplotlib** for training visualization.

## Future Work

- Extended dataset with more competitive programming platforms.
- Attempt direct integration with online judge systems.

## Citing This Work

If you use this dataset please cite, maybe I will add an actual PDF later:

```bibtex
@misc{dimitrov2025sgu,
  title={ACM SGU Competitive Programming Solutions with LLM Enhancement},
  author={Radoslav Dimitrov},
  year={2025},
}
```

## Acknowledgments

- SGU (Saratov State University) for the original problem set.
- OpenAI for the GPT o4-mini model used in editorial generation.
- Apple MLX team for the training framework.
- OlympicCoder project for the base model architecture.
