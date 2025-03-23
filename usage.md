# Ciphers in C

This repository contains two simple cipher programs implemented in C:

- **Caesar Cipher**  
  A simple substitution cipher that shifts each alphabetic character by a fixed amount. Non-alphabetic characters remain unchanged.

- **Scytale Cipher**  
  A transposition cipher that rearranges the text using a key (the number of columns) by writing the message in rows and then reading it column by column.

## File Structure

- `caesar.c` — Source code for the Caesar cipher program.
- `scytale.c` — Source code for the Scytale cipher program.
- `usage.md` — This file.

## Requirements

- A C compiler (e.g., GCC)

## Compilation Instructions

### Caesar Cipher

Compile the Caesar cipher program with:

```bash
gcc caesar.c -o caesar
```
### Scytale Cipher

Compile the Scytale cipher program with:
```bash
gcc scytale.c -o scytale
```
### Usage Instructions
## Caesar Cipher
# Usage:
```bash
./caesar <-e|-d> <shift> <input_file> <output_file>
```
-e: Encrypt mode.

-d: Decrypt mode.

<shift>: The shift value (an integer). The program normalizes the shift modulo 26 if necessary.

<input_file>: Path to the file containing the text to encrypt or decrypt.

<output_file>: Path to the file where the result will be saved.
Examples:

Encrypt with a shift of 3:

bash
Copy
./caesar -e 3 input.txt output.txt
Decrypt with a shift of 3:

bash
Copy
./caesar -d 3 input.txt output.txt
Scytale Cipher
Usage:

bash
Copy
./scytale <-e|-d> <key> <input_file> <output_file>
-e: Encrypt mode.

-d: Decrypt mode.

<key>: The key (a positive integer) representing the number of columns used in the transposition.

<input_file>: Path to the file containing the text to encrypt or decrypt.

<output_file>: Path to the file where the result will be saved.

# Examples:

 Encrypt with a key of 5:
```bash
./scytale -e 5 input.txt output.txt
```
# Decrypt with a key of 5:

```bash
./scytale -d 5 input.txt output.txt
```
### Program Details
## Caesar Cipher
# Encryption:
Reads each character from the input file and, if it's an alphabetical character, shifts it by the given value (wrapping around the alphabet). The case (upper or lower) is preserved.

# Decryption:
Shifts the characters in the opposite direction using the same key.

## Scytale Cipher
# Encryption:

Reads the entire input file into memory.

Arranges the text into rows with the number of columns equal to the key.

Reads the characters column by column to produce the ciphertext.

# Decryption:

Computes the number of rows based on the key and text length.

Fills a 2D table column by column with the ciphertext.

Reads the table row by row to reconstrsuct the original plaintext.