#include <ctype.h>
#include <stdio.h>

void f_pop(const u_char* packet, int* size, int* size_c){

    ascii(packet,size,*size_c);
    printf("\n\n");
}
