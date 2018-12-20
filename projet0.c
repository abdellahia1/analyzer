#include "projet0.h"


int comp=0;
struct in_addr ip_addr; //net
struct in_addr ip_mask;  //mask
int verbosity = 1;
char errbuf[PCAP_ERRBUF_SIZE]; //Error message


#define MAXBYTES 1518
#define TIMEOUT 1000


//how to use
void usage(char *program_name) {
  printf("Usage : %s (-i <interface> | -o <file>) [-f <BPF filter>] [-v <1|2|3>(verbosity)>]\n", program_name);
  exit(2);
}


//errors due to pcap
void stop_pcap(const char *error) {
    perror(error);
    fprintf(stderr, "%s\n", errbuf);
    exit(EXIT_FAILURE);
}

//catch a packet
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){

    int i;
    int size=0;
    int size_c;
    int pack;
    int packet_size=header->len;

    printf("\033[36m===================ETHERNET===================\n\n");
    printf("\033[00m");

    pack=f_ethernet(packet,&size); //Ethernet

    switch(pack){
        case IS_IP:
            printf("\033[36m    ======================IP======================\n\n");
            printf("\033[00m");
            pack=f_ip(packet,&size,&size_c);
            break;    //IPv4
        case IS_IP6:
            printf("\033[36m    =====================IPv6=====================\n\n");
            printf("\033[00m");
            pack=f_ipv6(packet,&size);
            break;  //IPv6
        case IS_ARP:
            printf("\033[36m    ======================ARP=====================\n\n");
            printf("\033[00m");
            pack=f_arp(packet, &size);
            break;  //ARP
    }

    switch(pack){
        case IS_TCP:
            printf("\033[36m        ======================TCP=====================\n\n");
            printf("\033[00m");
            f_tcp(packet, &size, &size_c);
            break;   //TCP
        case IS_UDP:
            printf("\033[36m        ======================UDP=====================\n\n");
            printf("\033[00m");
            f_udp(packet, &size);
            break;    //UDP
        case IS_ICMP:
            printf("\033[36m        =====================ICMP=====================\n\n");
            printf("\033[00m");
            pack=f_icmp(packet, &size);
            break;  //ICMP
        case IS_SCTP:
            printf("\033[36m        =====================SCTP=====================\n\n");
            printf("\033[00m");
            pack=f_sctp(packet, &size);
            break;   //SCTP
    }

}

//          ***********************************MAIN*******************************************


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
            //help for usage
            case 'h':
                usage(argv[0]);
                break;
            //sniffing device
            case 'i':
                device = optarg;
                break;
            //file
            case 'o':
                capture_file = optarg;
                break;
            case 'f':
            //filter
                filter = optarg;
                printf("using \"%s\" as BPF filter\n", filter);
                break;
            //verbosity (1 by default)
            case 'v':
                verbosity = atoi(optarg);
                if(verbosity < 1 || verbosity > 3) {
                    fprintf(stderr, "invalid verbosity level %d\n", verbosity);
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                break;
        }
    }

    //lookup for a device
    if(device != NULL && capture_file != NULL) {
        fprintf(stderr, "Error: you must choose between live capture or capture file\n");
        exit(EXIT_FAILURE);
    }

    if(capture_file == NULL) {
        if(device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
            stop_pcap("pcap_lookupdev");
        }
        printf("\nUsing device %s for capture\n", device);

        //lookup for ip adress and mask of network
        if(pcap_lookupnet(device, &ip, &mask, errbuf) != 0) {
            stop_pcap("pcap_lookupnet\n");
        }

        //print_ip_addr(ip); printf(" | Mask addr : "); print_ip_addr(mask); printf(")\n");

        //capture a apcket on device
        if((capture = pcap_open_live(device, 1500, 0, 100, errbuf)) == NULL) {
            stop_pcap("pcap_open_live");
        }
    }
    else {
        //if the file is given
        if((capture = pcap_open_offline(capture_file, errbuf)) == NULL) {
            stop_pcap("pcap_open_offline");
        }
    }

    if(pcap_compile(capture, &bpf_program, filter, 0, mask) != 0) {
        stop_pcap("pcap_compile");
    }

    if(pcap_setfilter(capture, &bpf_program) != 0) {
        stop_pcap("pcap_setfilter");
    }

    pcap_freecode(&bpf_program);

    pcap_loop(capture, -1, &got_packet, NULL);

    return EXIT_SUCCESS;
}

