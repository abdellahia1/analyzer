
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void PrintData(const u_char* data, int* size) {
    int i; int j;
    for (i = 0; i < *size; i++) {
        if (i != 0 && i % 16 == 0)
        {
            printf("       ");
            for (j = i - 16; j < i; j++) {
                if (data[j] >= 32 && data[j] <= 128)
                    printf("%c", (unsigned char) data[j]);

                else printf(".");
            }
            printf("\n");
        }

        if (i % 16 == 0) printf("       ");
        printf(" %02X", (unsigned int) data[i]);

        if (i == *size - 1)
        {
            for (j = 0; j < 15 - i % 16; j++) printf("   ");

            printf("         ");

            for (j = i - i % 16; j <= i; j++) {
                if (data[j] > 31 && data[j] < 128) printf("%c", (unsigned char) data[j]);
                else printf(".");
            }
            printf("\n");
        }
    }
}
