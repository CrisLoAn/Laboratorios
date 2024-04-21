#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>

/* This function will be invoked by pcap for each captured packet.
We can process each packet inside the function.
*/
void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet)
{
    struct ip *ip_header;
    // Check if the packet contains an Ethernet header
    if (header->caplen < sizeof(struct ether_header)) {
        fprintf(stderr, "Packet is too short to contain Ethernet header\n");
        return;
    }

    // Extract the IP header from the packet
    ip_header = (struct ip*)(packet + sizeof(struct ether_header));

    // Print the source and destination IP addresses
    printf("Source IP: %s\n", inet_ntoa(ip_header->ip_src));
    printf("Destination IP: %s\n", inet_ntoa(ip_header->ip_dst));
}

int main()
{
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "icmp";
    bpf_u_int32 net;
    // Step 1: Open live pcap session on NIC with name eth3.
    // Students need to change "eth3" to the name found on their own
    // machines (using ifconfig). The interface to the 10.9.0.0/24
    // network has a prefix "br-" (if the container setup is used).
    handle = pcap_open_live("br-524dc43e31c9", BUFSIZ, 1, 1000, errbuf);
    // Step 2: Compile filter_exp into BPF psuedo-code
    pcap_compile(handle, &fp, filter_exp, 0, net);
    if (pcap_setfilter(handle, &fp) !=0) {
        pcap_perror(handle, "Error:");
        (EXIT_FAILURE);
    }
    // Step 3: Capture packets
    pcap_loop(handle, -1, got_packet, NULL);
    pcap_close(handle); //Close the handle
    return 0;
}

