import os
import glob
import argparse
from openai import OpenAI


def generate_with_openai(client: OpenAI, prompt: str, problem_name: str) -> str:
    """
    Generates content for a given problem using the OpenAI API.
    """
    system_prompt = f"""
You are an expert programmer and problem solver.
The user will provide a concatenation of all files related to a programming problem, including the problem statement, sample inputs/outputs, and a solution file.
Based on all the information provided for problem `{problem_name}`, please provide the following:
1.  A concise, abridged version of the problem statement.
2.  A detailed editorial explaining the solution.
3.  The provided C++ solution, but with detailed comments explaining each line.
4.  A Python solution that solves the same problem, with detailed comments.
5.  A compressed editorial.
"""

    response = client.responses.create(
        model="gpt-5.1",
        input=[
            {"role": "system", "content": [{"type": "input_text", "text": system_prompt}]},
            {"role": "user", "content": [{"type": "input_text", "text": prompt}]},
        ],
    )
    return getattr(response, "output_text", "")

def extract_statement_from_raw(raw_content):
    """Extract the problem statement from raw file content."""
    import re
    statement_match = re.search(
        r"statement\.txt\n=+\n(.*?)(?:\n=+\n|$)", raw_content, re.DOTALL
    )
    if statement_match:
        return statement_match.group(1).strip()
    return ""

def generate_finetune_response_with_openai(client: OpenAI, statement_content: str, editorial_content: str, raw_content: str, problem_name: str) -> str:
    """
    Generates the response part of finetune format using OpenAI API with existing editorial and raw data.
    """
    prompt = f"""
I need you to create a structured solution for problem `{problem_name}`. 

Here is the problem statement:
{statement_content}

Here is an existing editorial with solutions:
{editorial_content}

Here is the raw problem data (including any sample inputs/outputs and original solution):
{raw_content}

Please create a response with the following structure:

1. An abridged problem statement (concise version)
2. Key observations needed to solve the problem  
3. Full solution approach based on the observations
4. C++ implementation with detailed comments
5. Python implementation with detailed comments

Use the provided editorial and solutions as reference, but create a clear, educational response that teaches how to solve this algorithmic problem step by step.
"""

    response = client.responses.create(
        model="gpt-5.1",
        input=[
            {"role": "user", "content": [{"type": "input_text", "text": prompt}]},
        ],
    )
    return getattr(response, "output_text", "")




def process_problem(client: OpenAI, problem_dir: str, dataset_dir: str):
    problem_name = os.path.basename(problem_dir)
    output_file = os.path.join(dataset_dir, f"{problem_name}.txt")
    raw_output_file = os.path.join(dataset_dir, f"{problem_name}_raw.txt")
    finetune_output_file = os.path.join(
        dataset_dir, f"{problem_name}_finetune.txt"
    )

    prompt = ""
    for file_path in glob.glob(os.path.join(problem_dir, "*.*")):
        if not os.access(file_path, os.X_OK):
            try:
                with open(
                    file_path, "r", encoding="utf-8", errors="ignore"
                ) as f:
                    content = f.read()
                    file_name = os.path.basename(file_path)
                    prompt += f"{file_name}\n======================\n{content}\n=================\n"
            except Exception as e:
                print(f"Could not read file {file_path}: {e}")

    if not os.path.exists(raw_output_file):
        with open(raw_output_file, "w", encoding="utf-8") as f:
            f.write(prompt)
        print(f"Generated raw prompt for {problem_name}")

    # Skip OpenAI call if finetune file already exists
    if os.path.exists(finetune_output_file):
        print(f"Skipping {problem_name}, finetune file already exists.")
        return

    if os.path.exists(output_file):
        print(f"Skipping {problem_name}, output already exists.")
    else:
        generated_content = generate_with_openai(client, prompt, problem_name)
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(generated_content)
        print(f"Processed {problem_name}")

    # Generate finetune format from raw content and editorial
    if os.path.exists(raw_output_file):
        try:
            # Read raw content and extract statement
            with open(raw_output_file, "r", encoding="utf-8") as f:
                raw_content = f.read()
            
            statement_content = extract_statement_from_raw(raw_content)
            if not statement_content:
                print(f"Warning: Could not extract statement from {problem_name}")
                return

            # Read editorial content if available
            editorial_content = ""
            if os.path.exists(output_file):
                with open(output_file, "r", encoding="utf-8") as f:
                    editorial_content = f.read()

            # Generate response using OpenAI with all available data
            response_content = generate_finetune_response_with_openai(
                client, statement_content, editorial_content, raw_content, problem_name
            )

            # Create the instruction part (only clean problem statement)
            instruction = f"Solve the below problem. The solution should start with an abridged problem statement. Then key observations. Then full solution based on the observations. Then C++ and Python implementations with comments.\n\n{statement_content}"
            
            # Combine into finetune format
            finetune_content = f"<|instruction|>\n{instruction}\n\n<|response|>\n{response_content}"

            with open(finetune_output_file, "w", encoding="utf-8") as f:
                f.write(finetune_content)
            print(f"Created finetune file for {problem_name}")

        except Exception as e:
            print(f"Error creating finetune file for {problem_name}: {e}")
    else:
        print(f"Missing raw file for finetune generation: {problem_name}")


def main():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_problems_dir = os.path.abspath(
        os.path.join(script_dir, "..", "problems")
    )
    default_dataset_dir = os.path.abspath(
        os.path.join(script_dir, "..", "dataset")
    )

    parser = argparse.ArgumentParser(
        description="Process SGU problems to generate a dataset."
    )
    parser.add_argument(
        "--problems_dir",
        type=str,
        default=default_problems_dir,
        help="The directory containing the SGU problems.",
    )
    parser.add_argument(
        "--dataset_dir",
        type=str,
        default=default_dataset_dir,
        help="The directory to save the generated dataset.",
    )
    parser.add_argument(
        "--openai_api_key",
        type=str,
        default=os.environ.get("OPENAI_API_KEY"),
        help="Your OpenAI API key.",
    )
    args = parser.parse_args()

    if not args.openai_api_key:
        raise ValueError(
            "OpenAI API key not found. Please set the OPENAI_API_KEY"
            " environment variable or provide it as a command-line argument."
        )

    # Use Responses API client with a generous timeout to avoid model timeouts.
    client = OpenAI(api_key=args.openai_api_key, timeout=600)

    if not os.path.exists(args.dataset_dir):
        os.makedirs(args.dataset_dir)

    for problem_dir in glob.glob(os.path.join(args.problems_dir, "p*")):
        if os.path.isdir(problem_dir):
            process_problem(client, problem_dir, args.dataset_dir)


if __name__ == "__main__":
    main()
