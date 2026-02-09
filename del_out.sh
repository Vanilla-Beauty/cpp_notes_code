#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Error: $1 is not a valid directory"
    exit 1
fi

cd "$1"

find . -type f -executable -not -name '*.sh' -not -name '.git*' -not -name '.vscode*'  -print -delete
echo "All executable files in '$1' have been removed."
