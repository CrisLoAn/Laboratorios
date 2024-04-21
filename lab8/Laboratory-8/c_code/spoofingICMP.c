#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/if_packet.h> // Agregar esta línea

#include "myheader.h"

unsigned short in_cksum(unsigned short *buf, int length) {
    unsigned short *w = buf;
    int nleft = length;
    int sum = 0;
    unsigned short temp = 0;

    // El algoritmo de suma de verificación
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(u_char *)(&temp) = *(u_char *)w;
        sum += temp;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void send_raw_ip_packet(struct ipheader *ip) {
    struct sockaddr_in dest_info;
    struct packet_mreq enable; // Esta estructura se define en <linux/if_packet.h>

    // Crear un socket de red en bruto
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    // Configurar la opción del socket
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));

    // Información del destino
    dest_info.sin_family = AF_INET;
    dest_info.sin_addr = ip->iph_destip;

    // Enviar el paquete
    sendto(sock, ip, ntohs(ip->iph_len), 0, (struct sockaddr *)&dest_info, sizeof(dest_info));
    close(sock);
}

int main() {
    char buffer[1500];
    memset(buffer, 0, 1500);

    // Configurar el encabezado ICMP
    struct icmpheader *icmp = (struct icmpheader *)(buffer + sizeof(struct ipheader));
    icmp->icmp_type = 8;
    icmp->icmp_chksum = 0;
    icmp->icmp_chksum = in_cksum((unsigned short *)icmp, sizeof(struct icmpheader));

    // Configurar el encabezado IP
    struct ipheader *ip = (struct ipheader *)buffer;
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_ttl = 20;
    ip->iph_sourceip.s_addr = inet_addr("10.9.0.6");
    ip->iph_destip.s_addr = inet_addr("1.2.3.4");
    ip->iph_protocol = IPPROTO_ICMP;
    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct icmpheader));
    printf("seq=%hu ", icmp->icmp_seq);
    printf("type=%u \n", icmp->icmp_type);
    send_raw_ip_packet(ip);

    return 0;
}

