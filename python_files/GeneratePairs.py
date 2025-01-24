import os
import random
import argparse

def clean_alignment(seq1, seq2):
    """
    Cleans a pair of aligned sequences by resolving redundant gaps,
    misaligned gaps, and ensuring residues align wherever possible,
    including accounting for complementary base pairs.
    """
    # Define complementary base pairs
    complements = {'A': 'T', 'T': 'A', 'C': 'G', 'G': 'C'}

    # Convert sequences to lists for easier manipulation
    seq1 = list(seq1)
    seq2 = list(seq2)

    i = 0
    while i < len(seq1):
        # If both positions are gaps, remove the column
        if seq1[i] == '-' and seq2[i] == '-':
            del seq1[i]
            del seq2[i]
            continue  # Skip incrementing, as lists have shifted

        # Handle cases of complementary misaligned gaps
        elif seq1[i] == '-' and seq2[i] in complements and i + 1 < len(seq1) and seq1[i + 1] == complements[seq2[i]]:
            # Shift the gap in seq1 to the right
            seq1[i], seq1[i + 1] = seq1[i + 1], seq1[i]
        elif seq2[i] == '-' and seq1[i] in complements and i + 1 < len(seq2) and seq2[i + 1] == complements[seq1[i]]:
            # Shift the gap in seq2 to the right
            seq2[i], seq2[i + 1] = seq2[i + 1], seq2[i]

        # Remove mismatched gaps with complementary bases
        elif seq1[i] == '-' and seq2[i] in complements:
            del seq1[i]
            del seq2[i]
            continue
        elif seq2[i] == '-' and seq1[i] in complements:
            del seq1[i]
            del seq2[i]
            continue

        i += 1  # Move to the next column

    # Join the lists back into strings
    return ''.join(seq1), ''.join(seq2)

def get_unique_filename(directory, base_name):
    """Generates a unique filename by appending a number if the file exists."""
    counter = 1
    file_path = os.path.join(directory, f"{base_name}{counter}.txt")
    while os.path.exists(file_path):
        counter += 1
        file_path = os.path.join(directory, f"{base_name}{counter}.txt")
    return file_path

def save_to_file(seq1, seq2, directory, base_name):
    """Saves a pair of sequences to a uniquely named file."""
    file_path = get_unique_filename(directory, base_name)
    with open(file_path, 'w') as f:
        f.write(seq1 + '\n')
        f.write(seq2 + '\n')
    print(f"Saved file: {file_path}")

def generate_pairs(file_name, desired_length):
    try:
        # Ensure desired_length is valid
        
        if desired_length not in [10, 100, 1000, 5000, 10000,15000,20000, 'max_length']:
            raise ValueError("desired_length must be one of [10, 100, 1000, 5000, 10000, 15000, 20000 or 'max_length']")

        # Read the file and extract sequences
        with open(file_name, 'r') as file:
            lines = file.readlines()

        sequences = [line.strip() for line in lines if not line.startswith(">")]

        if not sequences:
            raise ValueError("No valid sequences found in the input file.")

        if len(sequences) < 2:
            raise ValueError("Input file must contain at least two sequences.")
        
        # Determine the length to use for the pairs
        if desired_length == 'max_length':
            length = min(len(seq) for seq in sequences)
        else:
            length = desired_length

        # Seed the random number generator with the current time
        random.seed()

        # Select two random sequences 
        seq1, seq2 = random.sample(sequences, 2)

        # Truncate sequences to the desired length
        
        seq1 = seq1[:length]
        seq2 = seq2[:length]

        # Determine lengths for train and test splits
        train_length = int(len(seq1) * 0.7)
        test_length = length - train_length

        # Split sequences into train and test parts
        train_seq1, test_seq1 = seq1[:train_length], seq1[train_length:]
        train_seq2, test_seq2 = seq2[:train_length], seq2[train_length:]

        # Clean the alignments
        train_seq1, train_seq2 = clean_alignment(train_seq1, train_seq2)
        test_seq1, test_seq2 = clean_alignment(test_seq1, test_seq2)

        if len(train_seq1) == 0 or len(train_seq2) == 0 or len(test_seq1) == 0 or len(test_seq2) == 0:
            raise ValueError("One of the sequences after cleaning has a length of 0. Cannot save files.")

        # Ensure output directories exist
        train_dir = "TRAIN"
        test_dir = "TEST"
        os.makedirs(train_dir, exist_ok=True)
        os.makedirs(test_dir, exist_ok=True)

        # Save to files
        save_to_file(train_seq1, train_seq2, train_dir, "train")
        save_to_file(test_seq1, test_seq2, test_dir, "test")

        print("Generated train and test files with aligned sequences.")

    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    parser = argparse.ArgumentParser(description="Generate pairs from a parsed .fasta file.")
    parser.add_argument('-fn', '--file_name', type=str, required=True, help="Name of the .txt file containing sequences.")
    parser.add_argument('-len', '--desired_length', type=str, required=True, help="Desired length of the result pairs (10, 100, 1000, 5000, 10000, 15000, 20000 or 'max_length').")
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
