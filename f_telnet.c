#define TELCMDS
#define TELOPTS
#include "telnet.h"


void f_telnet(const u_char* packet, int* size){
        const unsigned char *end = packet + *size;

    while(packet < end) {
        if(*packet & 0xFF) {
            packet++;
            uint8_t command = *packet++;
            printf("        cmd %s (%u): ", TELCMD_OK(command) ? TELCMD(command) : "CMD?", command);
            switch(command) {
                case DO:
                case DONT:
                case WONT:
                case WILL: {
                    uint8_t option = *packet++;
                    printf("%s (%u)", TELOPT(option), option);
                    break;
                }
                case SB: {
                    uint8_t suboption = *packet++;
                    printf("%s (%u)", TELOPT(suboption), suboption);
                    switch(suboption) {
                        case TELOPT_TSPEED:
                            printf(" = %u", *packet++);
                            break;
                        case TELOPT_NAWS:
                            printf(" = %u x %u", ntohs(*(uint16_t*)&packet[0]), ntohs(*(uint16_t*)&packet[2]));
                            packet += 4;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case SE:
                    printf("End of suboptions");
                    break;
                default:
                    printf("Unknown command");
                    break;
            }
            putchar('\n');
            if(command == SE)
                break; // end of options
        }
        else {
            packet++;
        }
    }

}
