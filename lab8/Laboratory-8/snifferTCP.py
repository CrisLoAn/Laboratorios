#!/usr/bin/env python3
from scapy.all import *

print('Sniffing packets')

def print_pkt(pkt):
    print_pkt.num_packets += 1
    print("=========================== packet: {} ===================".format(print_pkt.num_packets))
    pkt.show()

print_pkt.num_packets = 0

pkt = sniff(iface='br-524dc43e31c9', filter='tcp and src host 10.9.0.6 and dst port 23', prn=print_pkt)

