#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void caesarEncrypt(FILE *fin, FILE *fout, int shift);
void caesarDecrypt(FILE *fin, FILE *fout, int shift);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <-e|-d> <shift> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Determine mode (-e or -d)
    int encryptMode = 0;  // 1 -> encrypt, 0 -> decrypt
    if (argv[1][0] == '-' && argv[1][1] == 'e') {
        encryptMode = 1;
    } else if (argv[1][0] == '-' && argv[1][1] == 'd') {
        encryptMode = 0;
    } else {
        fprintf(stderr, "First argument must be -e (encrypt) or -d (decrypt)\n");
        return 1;
    }

    // Parse shift
    int shift = atoi(argv[2]);
    if (shift >26 || shift<0) {
        shift = (shift % 26 + 26) % 26; 
    }

    // Open files
    FILE *fin = fopen(argv[3], "r");
    if (!fin) {
        perror("Error opening input file");
        return 1;
    }

    FILE *fout = fopen(argv[4], "w");
    if (!fout) {
        perror("Error opening output file");
        fclose(fin);
        return 1;
    }

    // Perform encryption or decryption
    if (encryptMode) {
        caesarEncrypt(fin, fout, shift);
    } else {
        caesarDecrypt(fin, fout, shift);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}

void caesarEncrypt(FILE *fin, FILE *fout, int shift) {
    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        if (isalpha(ch)) {
            // Preserve case
            char base = isupper(ch) ? 'A' : 'a';
            ch = (ch - base + shift) % 26 + base;
        }
        fputc(ch, fout);
    }
}

void caesarDecrypt(FILE *fin, FILE *fout, int shift) {
    // Decryption is just shifting in the opposite direction
    caesarEncrypt(fin, fout, -shift);
}
