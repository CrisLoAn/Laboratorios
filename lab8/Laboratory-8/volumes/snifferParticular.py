#!/usr/bin/env python3
from scapy.all import *

print('Sniffing packets')

def print_pkt(pkt):
    print_pkt.num_packets += 1
    print("=========================== packet: {} ===================".format(print_pkt.num_packets))
    print(pkt.summary())

print_pkt.num_packets = 0

pkt = sniff(iface='br-524dc43e31c9', filter='net 128.230.0.0/16', prn=print_pkt)

