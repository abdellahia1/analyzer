#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NULLCHAR 0x00
#define END_OF_ASCII 0x1F

void ascii(const u_char* packet, int* size, int size_c) {
    int i = 1;
    char pack=packet[*size];
    while(pack!= NULLCHAR && i<=size_c){
        pack=packet[*size];
        if(pack<=END_OF_ASCII && pack!=0x0d && pack!=0x0a && pack!=NULLCHAR) printf(".");
        else printf("%c",pack);
        *size+=1;
        i++;
    }
    *size+=1;
}
