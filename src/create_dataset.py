import json
import os
import glob
from pathlib import Path


def create_dataset():
    """
    Creates a dataset in JSONL format from the finetune files.
    """
    script_dir = os.path.dirname(os.path.realpath(__file__))
    dataset_dir = os.path.abspath(os.path.join(script_dir, "..", "dataset"))
    dataset_path = Path("dataset.jsonl")

    with open(dataset_path, "w", encoding="utf-8") as f:
        for finetune_file_path in sorted(glob.glob(os.path.join(dataset_dir, "p*_finetune.txt"))):
            problem_name = os.path.basename(finetune_file_path).replace("_finetune.txt", "")
            
            try:
                with open(finetune_file_path, "r", encoding="utf-8") as finetune_file:
                    content = finetune_file.read()
                
                # Split on the tags
                if "<|instruction|>" in content and "<|response|>" in content:
                    parts = content.split("<|instruction|>", 1)[1]
                    if "<|response|>" in parts:
                        instruction_part, response_part = parts.split("<|response|>", 1)
                        instruction = instruction_part.strip()
                        response = response_part.strip()
                        
                        data = {
                            "instruction": instruction,
                            "response": response,
                        }
                        f.write(json.dumps(data) + "\n")
                        print(f"Added {problem_name} to dataset")
                    else:
                        print(f"Warning: No <|response|> tag found in {problem_name}")
                else:
                    print(f"Warning: Missing tags in {problem_name}")
                    
            except Exception as e:
                print(f"Error processing {problem_name}: {e}")

    print(f"Dataset created at {dataset_path}")


if __name__ == "__main__":
    create_dataset()
