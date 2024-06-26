#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

// Define the IP header structure
struct ipheader {
    unsigned char iph_ihl : 4, iph_ver : 4;
    unsigned char iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned short int iph_flag : 3, iph_offset : 13;
    unsigned char iph_ttl;
    unsigned char iph_protocol;
    unsigned short int iph_chksum;
    struct in_addr iph_sourceip;
    struct in_addr iph_destip;
};

// Define the UDP header structure
struct udpheader {
    unsigned short int udp_sport;
    unsigned short int udp_dport;
    unsigned short int udp_len;
    unsigned short int udp_sum;
};

// Function to send a raw IP packet
void send_raw_ip_packet(struct ipheader* ip) {
    struct sockaddr_in dest_info;
    int enable = 1;
    // Step 1: Create a raw network socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    // Step 2: Set Socket option
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));

    // Step 3: Provide destination information
    dest_info.sin_family = AF_INET;
    dest_info.sin_addr = ip->iph_destip;

    // Step 4: Send the packet out
    sendto(sock, ip, ntohs(ip->iph_len), 0, (struct sockaddr *)&dest_info, sizeof(dest_info));
    close(sock);
}

// Main function
int main() {
    char buffer[1500];

    memset(buffer, 0, 1500);
    struct ipheader *ip = (struct ipheader *) buffer;
    struct udpheader *udp = (struct udpheader *) (buffer +
                                          sizeof(struct ipheader));

    // Step 1: Fill in the UDP data field.
    char *data = buffer + sizeof(struct ipheader) +
                         sizeof(struct udpheader);
    const char *msg = "DOR DOR!\n";
    int data_len = strlen(msg);
    strncpy(data, msg, data_len);

    // Step 2: Fill in the UDP header.
    udp->udp_sport = htons(12345);
    udp->udp_dport = htons(9090);
    udp->udp_len = htons(sizeof(struct udpheader) + data_len);
    udp->udp_sum = 0; // Many OSes ignore this field, so we do not calculate it.

    // Step 3: Fill in the IP header.
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_ttl = 20;
    ip->iph_sourceip.s_addr = inet_addr("10.9.0.5");
    ip->iph_destip.s_addr = inet_addr("10.9.0.6");
    ip->iph_protocol = IPPROTO_UDP; // The value is 17.
    ip->iph_len = htons(sizeof(struct ipheader) +
                       sizeof(struct udpheader) + data_len);

    // Step 4: Finally, send the spoofed packet
    send_raw_ip_packet(ip);

    return 0;
}

