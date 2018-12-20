

void f_http(const u_char* packet, int* size){

    if(strncmp("HTTP", (char *)packet, 5) == 0
       || strncmp("GET", (char *)packet, 3) == 0
       || strncmp("POST", (char *)packet, 4) == 0
       || strncmp("PUT", (char *)packet, 4) == 0
       || strncmp("DELETE", (char *)packet, 6) == 0
       || strncmp("HEAD", (char *)packet, 4) == 0) {

        while(*packet != '\n')
            putchar(*packet++);

        putchar('\n');
        packet++;

        while(strncmp("\r\n", (char *)packet, 2) != 0) { // print headers (<==> until we have double \n)
            printf("        ");

            while(*packet != '\n')
                putchar(*packet++);

            putchar('\n');
            packet++;
        }
    }
    else {
        printf("               [... HTTP(S) Content...]\n");
    }
  		printf("\n");

}

