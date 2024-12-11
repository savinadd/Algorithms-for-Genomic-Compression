#!/bin/bash

normalize_path() {
    local path="$1"

    # Replace backslashes with forward slashes
    path="${path//\\//}"

    if grep -qi microsoft /proc/version 2>/dev/null; then
        # windows drive letters to /mnt/<drive>/
        if [[ "$path" =~ ^([A-Za-z]):/(.*) ]]; then
            drive_letter="${BASH_REMATCH[1],}"  # Cdrive letter to lowercase
            path="/mnt/${drive_letter}/${BASH_REMATCH[2]}"
        fi
    else
        # onvert "C:/path" to "/c/path"
        if [[ "$path" =~ ^([A-Za-z]):/(.*) ]]; then
            drive_letter="${BASH_REMATCH[1],}"  # drive letter to lowercase
            path="/${drive_letter}/${BASH_REMATCH[2]}"
        fi
    fi

    echo "$path"
}

#  input FASTA file
read -r -p "Enter the path to the input FASTA file: " input_file_raw
input_file=$(normalize_path "$input_file_raw")

# input file exists
if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' not found!"
    exit 1
fi

# prompt user for output file or use a default
read -r -p "Enter the path for the output file (default: cleaned_sequence.txt): " output_file_raw
output_file_raw=${output_file_raw:-cleaned_sequence.txt}
output_file=$(normalize_path "$output_file_raw")

# ensure the output directory exists
output_dir=$(dirname "$output_file")
if [ ! -d "$output_dir" ]; then
    echo "Output directory '$output_dir' does not exist. Creating it..."
    mkdir -p "$output_dir" || { echo "Failed to create directory '$output_dir'."; exit 1; }
fi

grep -v '^>' "$input_file" | tr -d '\n' | tr -cd 'ACTGactg' | tr 'a-z' 'A-Z' > "$output_file"

echo "Cleaned sequence written to '$output_file'"