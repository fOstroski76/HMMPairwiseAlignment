def calculate_percentage_identity(file_path):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
            if len(lines) != 2:
                raise ValueError("The file must contain exactly two strings, each in one row.")
            
            string1 = lines[0].strip()
            string2 = lines[1].strip()
            
            min_length = min(len(string1), len(string2))
            max_length = max(len(string1), len(string2))
            
            matches = sum(1 for i in range(min_length) if string1[i] == string2[i])
            percentage_identity = (matches / max_length) * 100
            
            return percentage_identity
    except Exception as e:
        print(f"An error occurred: {e}")
        return None

# Example usage:
file_path = 'local_path'
pid = calculate_percentage_identity(file_path)
if pid is not None:
    print(f"Percentage Identity: {pid:.2f}%")