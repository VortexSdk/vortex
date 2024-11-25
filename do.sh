#!/bin/bash

# Check if xclip is installed
if ! command -v xclip &> /dev/null; then
    echo "Error: xclip is not installed. Install it using:"
    echo "sudo apt-get install xclip    # For Debian/Ubuntu"
    echo "sudo dnf install xclip        # For Fedora"
    echo "sudo pacman -S xclip          # For Arch"
    exit 1
fi

# Check if the vortex directory exists
if [ ! -d "./vortex" ]; then
    echo "Error: ./vortex directory not found"
    exit 1
fi

# Find all files and concatenate their contents with separators
find ./vortex -type f -print0 | while IFS= read -r -d '' file; do
    echo -e "\n=== File: $file ===\n"
    cat "$file"
    echo -e "\n=== End of $file ===\n"
done | xclip -selection clipboard

echo "All file contents have been copied to clipboard!"
