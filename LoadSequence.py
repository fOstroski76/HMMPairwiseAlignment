import os
from Bio import SeqIO
# https://www.hiv.lanl.gov/cgi-bin/NEWALIGN/align.cgi

def check_and_create_input_folder(directory):
    input_folder = os.path.join(directory, "input")
    if not os.path.exists(input_folder):
        os.makedirs(input_folder)
    return input_folder

def list_fasta_files(input_folder):
    return [f for f in os.listdir(input_folder) if f.endswith('.fasta')]

def choose_fasta_file(fasta_files):
    if len(fasta_files) == 1:
        return fasta_files[0]
    else:
        print("Multiple FASTA files found:")
        for i, file in enumerate(fasta_files):
            print(f"{i + 1}: {file}")
        choice = int(input("Enter the number of the file you want to load: ")) - 1
        return fasta_files[choice]

def load_fasta_sequences(file_path):
    with open(file_path, "r") as file:
        sequences = list(SeqIO.parse(file, "fasta"))
    return sequences


def check_and_create_output_folder(directory):
    output_folder = os.path.join(directory, "sequences")
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    return output_folder

def get_unique_output_file_path(output_folder):
    base_name = "sequences"
    extension = ".txt"
    counter = 1
    output_file_path = os.path.join(output_folder, f"{base_name}{extension}")
        
    while os.path.exists(output_file_path):
        counter += 1
        output_file_path = os.path.join(output_folder, f"{base_name}{counter}{extension}")
        
    return output_file_path

def main():
    directory = os.path.dirname(os.path.abspath(__file__))
    input_folder = check_and_create_input_folder(directory)
    output_folder = check_and_create_output_folder(directory)
    fasta_files = list_fasta_files(input_folder)
        
    if not fasta_files:
        print("No FASTA files found in the input folder.")
        return
        
    chosen_file = choose_fasta_file(fasta_files)
    file_path = os.path.join(input_folder, chosen_file)
    sequences = load_fasta_sequences(file_path)
        
    output_file_path = get_unique_output_file_path(output_folder)
    with open(output_file_path, "w") as output_file:
        for seq in sequences:
            output_file.write(f">{seq.id}\n{seq.seq}\n")

if __name__ == "__main__":
    main()