#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>

void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet)
{
    struct ip *ip_header;
    ip_header = (struct ip*)(packet + sizeof(struct ether_header));
    printf("Source IP: %s\n", inet_ntoa(ip_header->ip_src));
    printf("Destination IP: %s\n", inet_ntoa(ip_header->ip_dst));
}

int main()
{
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    //char filter_exp_icmp[] = "icmp && (host 10.9.0.5 && host 10.9.0.6)";
    char filter_exp_tcp[] = "tcp dst portrange 10-100";
    bpf_u_int32 net;

    handle = pcap_open_live("br-524dc43e31c9", BUFSIZ, 1, 1000, errbuf);
    pcap_compile(handle, &fp, filter_exp_tcp, 0, net);
    if (pcap_setfilter(handle, &fp) != 0) {
        pcap_perror(handle, "Error setting filter:");
        exit(EXIT_FAILURE);
    }
    pcap_loop(handle, -1, got_packet, NULL);
    pcap_close(handle);
    return 0;
}

	
