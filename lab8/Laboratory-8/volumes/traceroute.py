#!/usr/bin/env python3

from scapy.all import *
import sys

a=IP()
a.dst = '8.8.4.4'
a.ttl = int(sys.argv[1])
b = ICMP()
a = sr1(a/b)

print("Source:", a.src)
