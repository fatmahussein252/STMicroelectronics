#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <linux/icmp.h>

#include "layers.h"

// The IP structure constructor
IP_t *ip_construct(const u_char *packet)
{
	IP_t *temp_obj = (IP_t*) malloc(sizeof(IP_t));
	temp_obj->ip = (struct iphdr*)(packet + 14); // Skip Ethernet header (14 bytes)
	temp_obj->IP_Header = print_ip_header;
	
	return temp_obj;
}
// The IP structure destructor
void ip_destruct(IP_t *obj)
{
	free(obj);
}
// The IP header print method
void print_ip_header(IP_t *ip_obj)
{   
    printf("Packet captured:\n");
    printf("IP Header:\n");
    printf("   |-IP Version        : %d\n", (unsigned int)ip_obj->ip->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)ip_obj->ip->ihl, ((unsigned int)(ip_obj->ip->ihl)) * 4);
    printf("   |-Type Of Service   : %d\n", (unsigned int)ip_obj->ip->tos);
    printf("   |-IP Total Length   : %d Bytes(Size of Packet)\n", ntohs(ip_obj->ip->tot_len));
    printf("   |-Identification    : %d\n", ntohs(ip_obj->ip->id));
    printf("   |-TTL      : %d\n", (unsigned int)ip_obj->ip->ttl);
    printf("   |-Protocol : %d\n", (unsigned int)ip_obj->ip->protocol);
    printf("   |-Checksum : %d\n", ntohs(ip_obj->ip->check));
    printf("   |-Source IP        : %s\n", inet_ntoa(*(struct in_addr *)&ip_obj->ip->saddr));
    printf("   |-Destination IP   : %s\n", inet_ntoa(*(struct in_addr *)&ip_obj->ip->daddr));

}
// The TCP structure constructor
TCP_t *tcp_construct(const u_char *packet, IP_t *ip_obj)
{
	TCP_t *temp_obj = (TCP_t*) malloc(sizeof(TCP_t));
	temp_obj->tcp = (struct tcphdr*)(packet + 14 + ip_obj->ip->ihl * 4);
	temp_obj->TCP_Header = print_tcp_header;
	
	return temp_obj;
}
// The TCP structure destructor
void tcp_destruct(TCP_t *obj)
{
	free(obj);
}

// The TCP header print method
void print_tcp_header(TCP_t *tcp_obj)
{   
        printf("TCP Header:\n");
        printf("   |-Source Port      : %u\n", ntohs(tcp_obj->tcp->source));
        printf("   |-Destination Port : %u\n", ntohs(tcp_obj->tcp->dest));
        printf("   |-Sequence : %u\n", ntohs(tcp_obj->tcp->seq));
        printf("   |-Ack_seq : %u\n", ntohs(tcp_obj->tcp->ack_seq));

}
// APP layer protocol
APP_t *app_construct()
{
	APP_t *temp_obj = (APP_t*) malloc(sizeof(APP_t));
	temp_obj->APP_Proto = print_app_protocol;
	
	return temp_obj;
}
void app_destruct(APP_t *obj)
{
	free(obj);
}
void print_app_protocol(TCP_t *tcp_obj)
{
	if (ntohs(tcp_obj->tcp->source) == 80 || ntohs(tcp_obj->tcp->dest) == 80) {
        printf("HTTP Payload (Port 80)\n");
    	} else if (ntohs(tcp_obj->tcp->source) == 443 || ntohs(tcp_obj->tcp->dest) == 443) {
        printf("HTTPS Payload (Port 443)\n");
   	} else if (ntohs(tcp_obj->tcp->source) == 22 || ntohs(tcp_obj->tcp->dest) == 22) {
        printf("SSH Payload (Port 22)\n");
    	}

}


// The UDP structure constructor
UDP_t *udp_construct(const u_char *packet, IP_t *ip_obj)
{
	UDP_t *temp_obj = (UDP_t*) malloc(sizeof(UDP_t));
	temp_obj->udp = (struct udphdr*)(packet + 14 + ip_obj->ip->ihl * 4);
	temp_obj->UDP_Header = print_udp_header;
	
	return temp_obj;
}
// The UDP structure destructor
void udp_destruct(UDP_t *obj)
{
	free(obj);
}
// The UDP header print method
void print_udp_header(UDP_t *udp_obj)
{   
        printf("UDP Header:\n");
        printf("   |-Source Port      : %u\n", ntohs(udp_obj->udp->source));
        printf("   |-Destination Port : %u\n", ntohs(udp_obj->udp->dest));
        printf("   |-Length : %u\n", ntohs(udp_obj->udp->len));

}

// The ICMP structure constructor
ICMP_t *icmp_construct(const u_char *packet, IP_t *ip_obj)
{
	ICMP_t *temp_obj = (ICMP_t*) malloc(sizeof(ICMP_t));
	temp_obj->icmp = (struct icmphdr*)(packet + 14 + ip_obj->ip->ihl * 4);
	temp_obj->ICMP_Header = print_icmp_header;
	
	return temp_obj;
}
// The UDP structure destructor
void icmp_destruct(ICMP_t *obj)
{
	free(obj);
}
// The UDP header print method
void print_icmp_header(ICMP_t *icmp_obj)
{   
        printf("ICMP Header:\n");
        printf("   |-type      : %d\n", (unsigned int)icmp_obj->icmp->type);
        printf("   |-checksum : %d\n", ntohs(icmp_obj->icmp->checksum));
        printf("   |-id : %u\n", ntohs(icmp_obj->icmp->un.echo.id));
        printf("   |-sequence : %u\n", ntohs(icmp_obj->icmp->un.echo.sequence));

}


// filter packets
char *filter_type(int argc, char **argv)
{
	char *filter_exp = (char*)malloc(100*sizeof(filter_exp));
	if (argc == 5) { 
        snprintf(filter_exp, 100*sizeof(filter_exp), "ip host %s and port %s", argv[2], argv[4]);
    	} else if(argc == 3 && (strcmp(argv[1], "srcip") == 0) ){
    	snprintf(filter_exp, 100*sizeof(filter_exp), "ip src host %s", argv[2]);
    	} else if(argc == 3 && (strcmp(argv[1], "dstip") == 0) ){
    	snprintf(filter_exp, 100*sizeof(filter_exp), "ip dst host %s", argv[2]);
   	} else if(argc == 3 && (strcmp(argv[1], "srcport") == 0) ){
    	snprintf(filter_exp, 100*sizeof(filter_exp), "src port %s", argv[2]);
    	} else if(argc == 3 && (strcmp(argv[1], "dstport") == 0) ){
    	snprintf(filter_exp, 100*sizeof(filter_exp), "dst port %s", argv[2]);
    	} else {
    	filter_exp = "";
    	}
	return filter_exp;
}
