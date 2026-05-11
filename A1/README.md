<!--
* Group Work!
* Code completed by Dusan Barudzija.
* Connor Carroll assisting in demo and setup Github init for collaboration 
* in following assignments and labs.
Student Name: Dusan Barudzija
Student ID: [YOUR_STUDENT_ID]
Submission Date: 2026-05-04
File Name: README.md
Description: README for CMPT 360 Assignment 1 - userclean
-->

# CMPT 360 - Assignment 1: userclean

## Solution Logic

`userclean` reads usernames from standard input (one per line) and validates each one against a set of criteria. If valid, it echoes the username unchanged. If invalid, it applies a series of transformations in order:

1. Convert all uppercase letters to lowercase
2. Replace whitespace characters with underscores
3. Remove all characters that are not letters, digits, or underscores
4. Pad with characters from `"user"` if the result is shorter than 3 characters
5. Truncate to 16 characters if longer

After transformation, if the result still fails validation (e.g., starts with a digit or underscore, or is a reserved name like `root`/`admin`), the username is reported as **invalid and unfixable**.

Input is read dynamically with no hardcoded buffer limits.

## Compiling and Running

```bash
# Compile
make

# Run interactively
./userclean

# Run with piped input
echo "Bob123" | ./userclean

# Run with a file
./userclean < input.txt

# Run with valgrind
make valgrind
```

## Status

The program works correctly on all provided example cases and edge cases including:
- Reserved names (`root`, `admin`)
- Mixed-case input
- Inputs with special characters
- Inputs requiring padding
- Inputs requiring truncation
- Inputs starting with non-lowercase characters after transformation

No known errors or limitations.

## Academic Integrity

"I certify that this submission represents entirely my own work."
