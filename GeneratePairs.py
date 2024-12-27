import os
import random
import argparse

def clean_alignment(seq1, seq2):
    """
    Cleans a pair of aligned sequences by resolving redundant gaps
    and ensuring residues align wherever possible.
    """
    seq1 = list(seq1)
    seq2 = list(seq2)

    i = 0
    while i < len(seq1):
        if seq1[i] == '-' and seq2[i] == '-':
            del seq1[i]
            del seq2[i]
            continue

        elif seq1[i] == '-' and i + 1 < len(seq2) and seq2[i] != '-' and seq1[i + 1] == seq2[i]:
            seq1[i], seq1[i + 1] = seq1[i + 1], seq1[i]
        elif seq2[i] == '-' and i + 1 < len(seq1) and seq1[i] != '-' and seq2[i + 1] == seq1[i]:
            seq2[i], seq2[i + 1] = seq2[i + 1], seq2[i]

        i += 1

    return ''.join(seq1), ''.join(seq2)

def generate_pairs(file_name, desired_length):
    try:
        # Ensure desired_length is valid
        if desired_length not in [10, 100, 1000, 5000, 10000, 'max_length']:
            raise ValueError("desired_length must be one of [10, 100, 1000, 5000, 10000 or 'max_length']")

        # Read the file and extract sequences
        with open(file_name, 'r') as file:
            lines = file.readlines()

        sequences = [line.strip() for line in lines if not line.startswith(">")]

        if not sequences:
            raise ValueError("No valid sequences found in the input file.")

        # Determine the length to use for the pairs
        if desired_length == 'max_length':
            length = min(len(seq) for seq in sequences)
        else:
            length = desired_length

        # Check if the requested length exceeds the smallest sequence length
        if any(len(seq) < length for seq in sequences):
            raise ValueError(f"Desired length {length} exceeds the length of at least one sequence in the file.")

        # Randomly select two sequences
        seq1, seq2 = random.sample(sequences, 2)
        seq1, seq2 = seq1[:length], seq2[:length]

        # Process the pairs
        result = clean_alignment(seq1, seq2)

        # Create the output directory if it doesn't exist
        output_dir = 'parsed_pairs'
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        # Determine the output file name
        existing_files = os.listdir(output_dir)
        file_count = len(existing_files) + 1
        output_file = os.path.join(output_dir, f'parsed_pair{file_count}.txt')

        # Save the result to the output file
        with open(output_file, 'w') as out_file:
            out_file.write(result[0] + '\n')
            out_file.write(result[1] + '\n')

        print(f"Output successfully written to: {output_file}")

    except ValueError as ve:
        print(f"ValueError: {ve}")
    except FileNotFoundError:
        print(f"Error: File '{file_name}' not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

def main():
    parser = argparse.ArgumentParser(description="Generate pairs from a parsed .fasta file.")
    parser.add_argument('-fn', '--file_name', type=str, required=True, help="Name of the .txt file containing sequences.")
    parser.add_argument('-len', '--desired_length', type=str, required=True, help="Desired length of the result pairs (10, 100, 1000, 5000, 10000 or 'max_length').")
    args = parser.parse_args()

    file_name = os.path.join('sequences', args.file_name)
    desired_length = args.desired_length

    # Convert desired_length to an integer if it's numeric
    if desired_length.isdigit():
        desired_length = int(desired_length)

    try:
        generate_pairs(file_name, desired_length)
    except Exception as e:
        print(f"Failed to complete operation: {e}")

if __name__ == "__main__":
    main()
