#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>
#include <ctype.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#include "projet0.h"
//Ethernet
#define	IS_IP 0x0800
#define IS_ARP 0x0806
//Protocols

#define IS_POP 110
#define IS_FTP 21
#define IS_TELNET 23
#define IS_SCTP 132
#define IS_ICMP 0x01
#define IS_TCP 0x06
#define IS_UDP 0x11
#define IS_IP6 0x86DD
#define IS_BOOTP1 67
#define IS_BOOTP2 68
#define IS_DNS 53
#define IS_HTTP 80
#define IS_SMTP 25

#include "f_pop.c"
//#include "f_ftp.c"
#include "f_sctp.c"
#include "f_icmp.c"
#include "f_smtp.c"
#include "f_dns.c"
#include "f_bootp.c"
#include "f_http.c"
#include "f_udp.c"
#include "f_tcp.c"
#include "f_ipv6.c"
#include "f_arp.c"
#include "f_ip.c"
#include "f_ethernet.c"

int comp=0;
struct in_addr ip_addr;
struct in_addr ip_mask;

#define MAXBYTES 1518
#define TIMEOUT 1000



void usage(char *program_name) {
  printf("Usage : %s (-i <interface> | -o <file>) [-f <BPF filter>] [-v <1|2|3>(verbosity)>]\n", program_name);
  exit(2);
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){

    int i;
    int size=0;
    int pack;
    int packet_size=header->len;
    //Ethernet
    printf("\033[36m===================ETHERNET===================\n\n");
    printf("\033[00m");
    pack=f_ethernet(packet,&size);
    switch(pack){
        case IS_IP:
            printf("\033[36m    ======================IP======================\n\n");
            printf("\033[00m");
            pack=f_ip(packet,&size);
            break;
        case IS_IP6:
            printf("\033[36m    =====================IPv6=====================\n\n");
            printf("\033[00m");
            pack=f_ipv6(packet,&size);
            break;
        case IS_ARP:
            printf("\033[36m    ======================ARP=====================\n\n");
            printf("\033[00m");
            pack=f_arp(packet, &size);
            break;
    }

    switch(pack){
        //TCP
        case IS_TCP:
            printf("\033[36m        ======================TCP=====================\n\n");
            printf("\033[00m");
            f_tcp(packet, &size);
            break;
        //UDP
        case IS_UDP:
            printf("\033[36m        ======================UDP=====================\n\n");
            printf("\033[00m");
            f_udp(packet, &size);
            break;
        case IS_ICMP:
            printf("\033[36m        =====================ICMP=====================\n\n");
            printf("\033[00m");
            pack=f_icmp(packet, &size);
            break;
        case IS_SCTP:
            printf("\033[36m        =====================SCTP=====================\n\n");
            printf("\033[00m");
            pack=f_sctp(packet, &size);
            break;
    }

}

//          ***********************************MAIN*******************************************

int verbosity = 1;
char errbuf[PCAP_ERRBUF_SIZE]; // Message d'erreur

// Fonction qui permet de gérer les erreurs relatives à pcap
void abort_pcap(const char *error) {
    perror(error);
    fprintf(stderr, "%s\n", errbuf);
    exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
    char *device = NULL; // when capturing live
    char *capture_file = NULL; // when reading capture file
    pcap_t *capture;
    bpf_u_int32 ip;
    bpf_u_int32 mask = PCAP_NETMASK_UNKNOWN;
    const char *filter = "";
    struct bpf_program bpf_program;

    int option;
    while((option = getopt(argc, argv, "hi:o:f:v:")) != -1) {
        switch(option) {
            case 'h':
                fprintf(stderr, "usage: %s [-v verbosity] [-f filter] [-i interface]|[-o capture_file]\n", argv[0]);
                exit(EXIT_SUCCESS);
       // L'interface que l'on souhaite écouter
            case 'i':
                device = optarg;
                break;
	// Le fichier à décoder
            case 'o':
                capture_file = optarg;
                break;
            case 'f':
	// Filtrage
                filter = optarg;
                printf("using \"%s\" as BPF filter\n", filter);
                break;
	// Niveau de verbosité souhaité (par défault : 1)
            case 'v':
                verbosity = atoi(optarg);
                if(verbosity < 1 || verbosity > 3) {
                    fprintf(stderr, "invalid verbosity level %d\n", verbosity);
                    exit(EXIT_FAILURE);
                }
                break;
            default: break;
        }
    }

    if(device != NULL && capture_file != NULL) {
        fprintf(stderr, "Error: you must choose between live capture or capture file\n");
        exit(EXIT_FAILURE);
    }

    if(capture_file == NULL) {
        if(device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
            abort_pcap("pcap_lookupdev");
        }
        printf("\nUsing device %s for capture ( IP addr : ", device);

        if(pcap_lookupnet(device, &ip, &mask, errbuf) != 0) {
            abort_pcap("pcap_lookupnet");
        }

        //print_ip_addr(ip); printf(" | Mask addr : "); print_ip_addr(mask); printf(")\n");

        if((capture = pcap_open_live(device, 1500, 0, 100, errbuf)) == NULL) {
            abort_pcap("pcap_open_live");
        }
    }
    else {
	// Dans le cas où un fichier est fourni, on le traite
        if((capture = pcap_open_offline(capture_file, errbuf)) == NULL) {
            abort_pcap("pcap_open_offline");
        }
    }

    if(pcap_compile(capture, &bpf_program, filter, 0, mask) != 0) {
        abort_pcap("pcap_compile");
    }

    if(pcap_setfilter(capture, &bpf_program) != 0) {
        abort_pcap("pcap_setfilter");
    }

    pcap_freecode(&bpf_program);

    pcap_loop(capture, -1, &got_packet, NULL);

    return EXIT_SUCCESS;
}

