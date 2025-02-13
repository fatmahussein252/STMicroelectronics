wireFish is a simple packet sniffer program in C using the Packet Capture library. The aim of this task is to apply OOP concepts in C.

## Features 
- Digest IP packets and show their fields.
- Digest the following TCP layer protocols (TCP, UDP, and ICMP).
- Digest the following application layer protocols (HTTP, HTTPS, and SSH).
- Support filtering on specific IP/port for source or distination or both using a command line option.


## How to use 
1) compile the program:
```
gcc wireFish.c layers.c -lpcap
```
2) Run it as sudo user
```
sudo ./a.out
```
To filter on specific ip/port
```
sudo ./a.out srcip <ip> destip <ip> srcport <port> destport <port> # can use any of them individually
``` 
