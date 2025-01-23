import random
import os

def generate_random_pairs(length, num_differences):
    # Define the possible characters
    characters = ['A', 'C', 'T', 'G']
    
    # Generate the initial identical rows
    row1 = ''.join(random.choice(characters) for _ in range(length))
    row2 = row1
    
    # Introduce differences in the first row
    row1_list = list(row1)
    for _ in range(num_differences):
        index = random.randint(0, length - 1)
        original_char = row1_list[index]
        new_char = random.choice([char for char in characters if char != original_char])
        row1_list[index] = new_char
    
    row1 = ''.join(row1_list)
    
    return row1, row2

# Example usage
length = int(input("Enter the desired length: "))
num_differences = int(input("Enter the number of differences: "))
row1, row2 = generate_random_pairs(length, num_differences)
#print("Row 1:", row1)
#print("Row 2:", row2)

# Create the directory if it doesn't exist
directory = "random_sequences"

if not os.path.exists(directory):
    os.makedirs(directory)

# Determine the next available file number
file_number = 1
while os.path.exists(os.path.join(directory, f"randomsequence{file_number}.txt")):
    file_number += 1

# Save the rows to the file
file_path = os.path.join(directory, f"randomsequence{file_number}.txt")
with open(file_path, 'w') as file:
    file.write(f"{row1}\n")
    file.write(f"{row2}\n")

print(f"Rows saved to {file_path}")