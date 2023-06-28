#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

void perro(const char *msg) {
    perror(msg);
    exit(1);
}

void compress_string(const char* input, uLongf inputLen, Bytef* compressed, uLongf* compressedLen) {
    compress(compressed, compressedLen, (const Bytef*)input, inputLen);
}

void decompress_string(const Bytef* compressed, uLongf compressedLen, char* decompressed, uLongf* decompressedLen) {
    uncompress((Bytef*)decompressed, decompressedLen, compressed, compressedLen);
}

int main() {
    const char* originalString = "Texto original. dasnkdnamskldmkla sdankldasl  dklas jdasndlaskdlkasd asndlkasndlnasl dnassldnlaks n";
    uLongf originalLen = strlen(originalString) + 1;

    printf("Original String (length: %lu bytes)\n", originalLen);

    uLongf compressedLen = compressBound(originalLen);
    Bytef* compressed = (Bytef*) malloc(compressedLen * sizeof(Bytef));
    compress_string(originalString, originalLen, compressed, &compressedLen);

    printf("Compressed String (length: %lu bytes): ", compressedLen);
    for (uLongf i = 0; i < compressedLen; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");

    uLongf decompressedLen = originalLen;
    char* decompressed = (char*) malloc(decompressedLen * sizeof(char));
    decompress_string(compressed, compressedLen, decompressed, &decompressedLen);

    printf("Decompressed String (length: %lu bytes): %s\n", decompressedLen, decompressed);

    printf("Original Size: %lu bytes\n", originalLen);
    printf("Compressed Size: %lu bytes\n", compressedLen);
    printf("Compression Ratio: %.2f%%\n", ((float)(originalLen - compressedLen) / originalLen) * 100);

    free(compressed);
    free(decompressed);

    return 0;
}


