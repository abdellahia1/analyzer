

void f_ftp(const u_char* packet, int* size, int* size_c){

        ascii(packet,size,*size_c);
  		printf("\n");

}


/*
 if(strncmp("FTP", (char *)packet, 3) == 0
       || strncmp("USER", (char *)packet, 4) == 0
       || strncmp("PASS", (char *)packet, 4) == 0
       || strncmp("ACCT", (char *)packet, 4) == 0
       || strncmp("PORT", (char *)packet, 4) == 0
       || strncmp("PASV", (char *)packet, 4) == 0
       || strncmp("DELE", (char *)packet, 4) == 0
       || strncmp("LIST", (char *)packet, 4) == 0
       || strncmp("HELP", (char *)packet, 4) == 0
       || strncmp("NOOP", (char *)packet, 4) == 0
       || strncmp("QUIT", (char *)packet, 4) == 0) {

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
        printf("                         [... FTP Content...]\n\n");
    }
*/
