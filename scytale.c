#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* scytaleEncrypt(const char *plaintext, int key);
char* scytaleDecrypt(const char *ciphertext, int key);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <-e|-d> <key> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Determine mode (-e or -d)
    int encryptMode = 0; // 1 for encrypt, 0 for decrypt
    if (strcmp(argv[1], "-e") == 0) {
        encryptMode = 1;
    } else if (strcmp(argv[1], "-d") == 0) {
        encryptMode = 0;
    } else {
        fprintf(stderr, "First argument must be -e (encrypt) or -d (decrypt)\n");
        return 1;
    }

    // Parse key (number of columns)
    int key = atoi(argv[2]);
    if (key < 1) {
        fprintf(stderr, "Key must be a positive integer.\n");
        return 1;
    }

    // Read the entire input file into memory
    FILE *fin = fopen(argv[3], "r");
    if (!fin) {
        perror("Error opening input file");
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char *text = (char*)malloc(fsize + 1);
    if (!text) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(fin);
        return 1;
    }
    fread(text, 1, fsize, fin);
    text[fsize] = '\0';
    fclose(fin);

    // Open output file
    FILE *fout = fopen(argv[4], "w");
    if (!fout) {
        perror("Error opening output file");
        free(text);
        return 1;
    }

    //Encrypt or decrypt
    char *result = NULL;
    if (encryptMode) {
        result = scytaleEncrypt(text, key);
    } else {
        result = scytaleDecrypt(text, key);
    }

    if (result) {
        fprintf(fout, "%s", result);
        free(result);
    }

    free(text);
    fclose(fout);

    return 0;
}

/* Encryption:
 *  1. Write the plaintext in rows of length = key.
 *  2. Read column by column to produce ciphertext.*/
char* scytaleEncrypt(const char *plaintext, int key) {
    int len = strlen(plaintext);
    if (len == 0) {
        /* Return empty string if nothing to encrypt */
        char *empty = (char*)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    // Number of rows needed if each row has 'key' columns
    int rows = (len + key - 1) / key;

    // Allocate memory for ciphertext
    char *encrypted = (char*)malloc(len + 1);
    int idx = 0;

    // Read column by column
    for (int col = 0; col < key; col++) {
        for (int row = 0; row < rows; row++) {
            int pos = row * key + col;
            if (pos < len) {
                encrypted[idx++] = plaintext[pos];
            }
        }
    }
    encrypted[idx] = '\0';

    return encrypted;
}

/* Decryption:
 *  1. We know the ciphertext was written column by column (with 'key' columns).
 *  2. Fill a 2D table column by column with the ciphertext.
 *  3. Read row by row to get the original plaintext. */
char* scytaleDecrypt(const char *ciphertext, int key) {
    int len = strlen(ciphertext);
    if (len == 0) {
        // Return empty string if nothing to decrypt
        char *empty = (char*)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    // Calculate number of rows used during encryption
    int rows = (len + key - 1) / key;

    // Create a 2D table to reconstruct the text
    char **table = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        table[i] = (char*)malloc(key * sizeof(char));
        // Initialize to '\0' so we can tell empty cells apart
        for (int j = 0; j < key; j++) {
            table[i][j] = '\0';
        }
    }

    // Fill the table column by column with the ciphertext
    int idx = 0;
    for (int col = 0; col < key; col++) {
        for (int row = 0; row < rows; row++) {
            int pos = row * key + col;
            if (pos < len) {
                table[row][col] = ciphertext[idx++];
            }
        }
    }

    // Read row by row to get the plaintext
    char *plaintext = (char*)malloc(len + 1);
    idx = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < key; col++) {
            if (table[row][col] != '\0') {
                plaintext[idx++] = table[row][col];
            }
        }
    }
    plaintext[idx] = '\0';

    // Cleanup
    for (int i = 0; i < rows; i++) {
        free(table[i]);
    }
    free(table);

    return plaintext;
}
