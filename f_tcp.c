

void f_tcp(const u_char* packet, int* size, int* size_c) {
    u_int data_offset;
    struct tcphdr* tcphead = (struct tcphdr*)( packet + *size);

    *size+= 4 * tcphead->th_off;
    *size_c-= 4 * tcphead->th_off;

    printf("\033[35m        Source port : %i\n", ntohs(tcphead->th_sport));
    printf("        Destination port : %i\n", ntohs(tcphead->th_dport));
    printf("        Sequence number : 0x%.4x\n", ntohs(tcphead->th_seq));
    printf("        Acknowledgment number : 0x%.4x\n", ntohs(tcphead->th_ack));
    printf("        Sequence number : 0x%.4x\n", ntohs(tcphead->th_seq));
    printf("        Data offset : %i bytes\n", tcphead->th_off*4);
    printf("        Unused : %i\n", tcphead->th_x2*4);
    printf("        Flags : (0x%.4x)", tcphead->th_flags);
    switch(tcphead->th_flags){
        case TH_FIN:
        printf(" FIN\n");
        break;
        case TH_SYN:
        printf(" SYN\n");
        break;
        case TH_RST:
        printf(" RST\n");
        break;
        case TH_PUSH:
        printf(" PSH\n");
        break;
        case TH_ACK:
        printf(" ACK\n");
        break;
        case TH_URG:
        printf(" URG\n");
        break;
    }
    printf("        Window : %i\n", ntohs(tcphead->th_win));
    printf("        Checksum : 0x%.4x\n", ntohs(tcphead->th_sum));
    printf("        Urgent pointer : %i\n", ntohs(tcphead->th_urp)); /*0x%.4x*/
    printf("        \033[00m");

    printf("\n");










    if(size_c != 0) {
        //BOOTP
        if((ntohs(tcphead->th_sport)==IS_BOOTP1 && ntohs(tcphead->th_dport)==IS_BOOTP2)
          || (ntohs(tcphead->th_sport)==IS_BOOTP2 && ntohs(tcphead->th_dport)==IS_BOOTP1))
        {
            printf("\033[36m            ==================BOOTP/DHCP==================\n\n");
            printf("\033[00m");
            f_bootp(packet,size);
        }
        //DNS
        if((ntohs(tcphead->th_sport)==IS_DNS || ntohs(tcphead->th_dport)==IS_DNS))
        {
            printf("\033[36m            ======================DNS=====================\n");
            printf("\033[00m");
            f_dns(packet,size);
        }
        //HTTP
        if((ntohs(tcphead->th_sport)==IS_HTTP || ntohs(tcphead->th_dport)==IS_HTTP))
        {
            printf("\033[36m\n            ======================HTTP=====================\n");
            printf("\033[00m");
            f_http(packet,size,size_c);
        }
        //SMTP
        if((ntohs(tcphead->th_sport)==IS_SMTP || ntohs(tcphead->th_dport)==IS_SMTP))
        {
            printf("\033[36m            ======================SMTP=====================\n");
            printf("\033[00m");
            f_smtp(packet,size,size_c);
        }
        //TELNET
        if((ntohs(tcphead->th_sport)==IS_TELNET || ntohs(tcphead->th_dport)==IS_TELNET))
        {
            printf("\033[36m            =====================TELNET====================\n");
            printf("\033[00m");
            f_telnet(packet,size);
        }
        //FTP
        if((ntohs(tcphead->th_sport)==IS_FTP || ntohs(tcphead->th_dport)==IS_FTP))
        {
            printf("\033[36m            =======================FTP=====================\n");
            printf("\033[00m");
            f_ftp(packet,size,size_c);
        }
        //POP
        if((ntohs(tcphead->th_sport)==IS_POP || ntohs(tcphead->th_dport)==IS_POP))
        {
            printf("\033[36m            =======================POP=====================\n");
            printf("\033[00m");
            f_pop(packet,size,size_c);
        }
        //IMAP
        if((ntohs(tcphead->th_sport)==IS_IMAP || ntohs(tcphead->th_dport)==IS_IMAP))
        {
            printf("\033[36m            ======================IMAP=====================\n");
            printf("\033[00m");
            f_imap(packet,size,size_c);
        }
    }
}
