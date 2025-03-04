#!/bin/bash

# Check if input is given
if [ $# -ne 1 ]; then
    echo "Usage: $0 input-file"
    exit 1
fi

in_file="$1"
out_file="output.txt"

# Clear the output file if it exists
> "$out_file"

# Read the input file line by line and extract desired fields
while IFS= read -r line; do
    for field in "frame.time" "wlan.fc.type" "wlan.fc.subtype"; do
        if [[ "$line" == *"$field"* && "$line" != *"${field}_"* ]]; then
            value=$(echo "$line" | awk -F': ' '{print $2}' | tr -d '"')
            echo "\"$field\": \"$value\"" >> "$out_file"
            break
        fi
    done
done < "$in_file"

# Output completion message
echo "Output saved to $out_file"
