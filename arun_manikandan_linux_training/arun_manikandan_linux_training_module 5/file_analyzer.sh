#!/bin/bash

# Redirect stderr to both errors.log and terminal
exec 2> >(tee -a errors.log >&2)

# Function to display help
usage() {
    cat << HELP
File Analyzer Help:
Usage: $0 [OPTIONS]

Options:
-d <directory>  Directory to search recursively
-k <keyword>    Keyword to search for (non-empty)
-f <file>       File to search directly
--help          Display this help menu

Examples:
$0 -d /path/to/dir -k "search term"
$0 -f /path/to/file -k "search term"
HELP
}

# Error logging function
log_error() {
    local message="$1"
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $0 - ERROR: $message" >> errors.log
    echo "$0: Error: $message" >&2
}

# Check for --help first
for arg in "$@"; do
    if [[ "$arg" == "--help" ]]; then
        usage
        exit 0
    fi
done

# Check if no arguments provided
if [[ $# -eq 0 ]]; then
    log_error "No arguments provided. Use --help for usage."
    exit 1
fi

# Initialize variables
directory=""
keyword=""
file=""

# Parse options with getopts
while getopts ":d:k:f:" opt; do
    case $opt in
        d) directory="$OPTARG" ;;
        k) keyword="$OPTARG" ;;
        f) file="$OPTARG" ;;
        \?) log_error "Invalid option: -$OPTARG"; exit 1 ;;
        :) log_error "Option -$OPTARG requires an argument."; exit 1 ;;
    esac
done

# Validate keyword using regex (non-empty)
if [[ ! "$keyword" =~ ^.+$ ]]; then
    log_error "Keyword is empty or invalid."
    exit 1
fi

# Check mutual exclusivity of directory and file
if [[ -n "$directory" && -n "$file" ]]; then
    log_error "Cannot specify both directory (-d) and file (-f)."
    exit 1
elif [[ -z "$directory" && -z "$file" ]]; then
    log_error "Either directory (-d) or file (-f) must be specified."
    exit 1
fi

# Validate directory exists
if [[ -n "$directory" ]]; then
    if [[ ! -d "$directory" ]]; then
        log_error "Directory does not exist: $directory"
        exit 1
    fi
fi

# Validate file exists
if [[ -n "$file" ]]; then
    if [[ ! -f "$file" ]]; then
        log_error "File does not exist: $file"
        exit 1
    fi
fi

# Recursive search function
search_directory() {
    local dir="$1"
    local keyword="$2"
    for item in "$dir"/*; do
        if [[ -f "$item" ]]; then
            if grep -q "$keyword" "$item" 2>/dev/null; then
                echo "Found in file: $item"
            fi
        elif [[ -d "$item" ]]; then
            search_directory "$item" "$keyword"
        fi
    done
}

# Perform search based on options
if [[ -n "$directory" ]]; then
    search_directory "$directory" "$keyword"
elif [[ -n "$file" ]]; then
    # Using here-string to search keyword in file content
    content=$(<"$file")
    if grep -q "$keyword" <<< "$content"; then
        echo "Found in file: $file"
    else
        grep_exit_status=$?
        if [[ $grep_exit_status -eq 1 ]]; then
            echo "Keyword not found in file: $file"
        else
            log_error "Error searching file: $file (exit code $grep_exit_status)"
            exit 1
        fi
    fi
fi

exit 0
