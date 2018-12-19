
#include <ctype.h>
#include <stdio.h>




void f_pop(const u_char* packet, int* size){
    int i = 0;
	while (i<*size){
		if (i%47==0){
			printf("\n\t\t");
		}
		if(isprint(packet[i])){
			printf("%c", packet[i]);
		}
		else{
			printf(".");
		}
		i++;
	}
    printf("\n");
}
