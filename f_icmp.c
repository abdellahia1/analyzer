#include "icmp.h"
#include "data.h"

int f_icmp(const u_char* packet, int* size){
    unsigned short iphdrlen;

    struct ip* iphead = (struct ip *) packet;
    iphdrlen = iphead->ip_hl * 4;
    struct icmphdr *icmph = (struct icmphdr *) (packet +*size);

    *size+=sizeof(struct icmphdr);

    printf("\033[36m");
    printf("        Type : ");
    switch(icmph->type) {
        case ICMP_ECHOREPLY:
            printf("Echo Reply");
            break;
        case ICMP_DEST_UNREACH:
            printf("Destination Unreachable");
            break;
        case ICMP_SOURCE_QUENCH:
            printf("Source Quench");
            break;
        case ICMP_REDIRECT:
            printf("Redirect (change route)");
            break;
        case ICMP_ECHO:
            printf("Echo Request");
            break;
        case ICMP_TIME_EXCEEDED:
            printf("Time Exceeded");
            break;
        case ICMP_PARAMETERPROB:
            printf("Parameter Problem");
            break;
        case ICMP_TIMESTAMP:
            printf("Timestamp Request");
            break;
        case ICMP_TIMESTAMPREPLY:
            printf("Timestamp Reply");
            break;
        case ICMP_INFO_REQUEST:
            printf("Information Request");
            break;
        case ICMP_INFO_REPLY:
            printf("/* Information Reply");
            break;
        case ICMP_ADDRESS:
            printf("Address Mask Request");
            break;
        case ICMP_ADDRESSREPLY:
            printf("Address Mask Reply");
            break;
        default:
            printf("Unknown type");
    }

    printf(" (%u)\n", icmph->type);

    printf("        Code : %d\n", (unsigned int) (icmph->code));
    printf("        Checksum : 0x%.4x\n", ntohs(icmph->checksum));
    printf("        Identifier : %d (0x%.4x)\n", ntohs(icmph->un.echo.id), ntohs(icmph->un.echo.id));
    printf("        Sequence : %d (0x%.4x)\n", ntohs(icmph->un.echo.sequence), ntohs(icmph->un.echo.sequence));
    //printf("        Data Payload\n");
    //PrintData(packet, size);
    printf("\033[00m");
    printf("\n");
    return(0);
}


