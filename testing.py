def clean_alignment(seq1, seq2):
    """
    Cleans a pair of aligned sequences by resolving redundant gaps
    and ensuring residues align wherever possible.
    """
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

        # If there's a misaligned gap (e.g., -G vs G-)
        elif seq1[i] == '-' and i + 1 < len(seq2) and seq2[i] != '-' and seq1[i + 1] == seq2[i]:
            # Shift the gap in seq1 to the right
            seq1[i], seq1[i + 1] = seq1[i + 1], seq1[i]
        elif seq2[i] == '-' and i + 1 < len(seq1) and seq1[i] != '-' and seq2[i + 1] == seq1[i]:
            # Shift the gap in seq2 to the right
            seq2[i], seq2[i + 1] = seq2[i + 1], seq2[i]

        i += 1  # Move to the next column

    # Join the lists back into strings
    return ''.join(seq1), ''.join(seq2)


# Example usage
seq1 = "-G"
seq2 = "G-"
cleaned_seq1, cleaned_seq2 = clean_alignment(seq1, seq2)
print("Cleaned Alignment:")
print(cleaned_seq1)
print(cleaned_seq2)
