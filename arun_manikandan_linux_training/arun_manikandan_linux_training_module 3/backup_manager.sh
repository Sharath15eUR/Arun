#!/bin/bash

# Validating if 3 arguments are provided or not

if [ "$#" -ne 3 ]; then
	echo "Usage $0 <source_directory> <backup_directory> <file_extension>"
	exit 1
fi

# Creating variables for easier access to the CLI arguments

SOURCE_DIR="$1"
BACKUP_DIR="$2"
FILE_EXTENSION="$3"

# Check if SORUCE_DIR Exists or not

if [ ! -d "$SOURCE_DIR" ]; then
	echo "Source directory $SOURCE_DIR does not exist"
	exit 1
fi

# Globbing to find the files matching extensions in the SOURCE_DIR

FILES=($SOURCE_DIR/*$FILE_EXTENSION)

# Check if atleast one such file with the provided extension exists or not

if [ ${#FILES[@]} -eq 0 ]; then
    echo "No files with extension $FILE_EXTENSION found in $SOURCE_DIR."
    exit 1
fi

# Create and EXPORT the BACKUP_COUNT variable to 0

export BACKUP_COUNT=0

# Check if the BACKUP_DIR Exists or not if not then create one

if [ ! -d "$BACKUP_DIR" ]; then
    mkdir -p "$BACKUP_DIR"
    if [ $? -ne 0 ]; then
        echo "Failed to create backup directory $BACKUP_DIR."
        exit 1
    fi
fi

# Create an array to store files to be backed up

echo "Files to be backed up:"
TOTAL_SIZE=0

for FILE in "${FILES[@]}"; do
    FILE_SIZE=$(stat -c %s "$FILE")
    TOTAL_SIZE=$((TOTAL_SIZE + FILE_SIZE))
    FILE_NAME=$(basename "$FILE")
    echo "$FILE_NAME - $FILE_SIZE bytes"
done

# Perform backup

for FILE in "${FILES[@]}"; do
    FILE_NAME=$(basename "$FILE")
    DEST="$BACKUP_DIR/$FILE_NAME"

    # If the file exists in the backup directory then check if it's older than the source file

    if [ -f "$DEST" ]; then
        SOURCE_MOD_TIME=$(stat -c %Y "$FILE")
        DEST_MOD_TIME=$(stat -c %Y "$DEST")
        
        if [ "$SOURCE_MOD_TIME" -gt "$DEST_MOD_TIME" ]; then
            echo "Overwriting $DEST with $FILE"
            cp "$FILE" "$DEST"
            export BACKUP_COUNT=$((BACKUP_COUNT + 1))
        else
            echo "Skipping $FILE (backup already up to date)"
        fi
    else
        echo "Backing up $FILE"
        cp "$FILE" "$DEST"
        export BACKUP_COUNT=$((BACKUP_COUNT + 1))
    fi
done


# Generate the backup report

REPORT_FILE="$BACKUP_DIR/backup_report.log"
echo "Backup Summary" > "$REPORT_FILE"
echo "===================" >> "$REPORT_FILE"
echo "Total files processed: $BACKUP_COUNT" >> "$REPORT_FILE"
echo "Total size of files backed up: $TOTAL_SIZE bytes" >> "$REPORT_FILE"
echo "Backup directory: $BACKUP_DIR" >> "$REPORT_FILE"

echo "Backup completed. Report saved to $REPORT_FILE."
