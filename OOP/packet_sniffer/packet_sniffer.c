#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <linux/icmp.h>
#include "layers.h"


// Callback function called by pcap for each captured packet
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
   struct IP *ip_obj;
   ip_obj = ip_construct(packet);
   ip_obj->IP_Header(ip_obj);
   
   if (ip_obj->ip->protocol == IPPROTO_TCP) {
   	struct TCP *tcp_obj;
   	tcp_obj = tcp_construct(packet, ip_obj);
   	tcp_obj->TCP_Header(tcp_obj);
   	
   	struct APP *app_obj;
   	app_obj = app_construct();
   	app_obj->APP_Proto(tcp_obj);
   	
   	tcp_destruct(tcp_obj);
   	app_destruct(app_obj);
       
    } else if (ip_obj->ip->protocol == IPPROTO_UDP) {
    	struct UDP *udp_obj;
   	udp_obj = udp_construct(packet, ip_obj);
   	udp_obj->UDP_Header(udp_obj);
   	udp_destruct(udp_obj);
   	
    }
    if (ip_obj->ip->protocol == IPPROTO_ICMP) {
   	struct ICMP *icmp_obj;
   	icmp_obj = icmp_construct(packet, ip_obj);
   	icmp_obj->ICMP_Header(icmp_obj);
   	icmp_destruct(icmp_obj);
    }
    
    ip_destruct(ip_obj);
    printf("\n");
}

int main(int argc, char **argv) {
    char *dev = "wlo1"; // Change this to your network interface
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    char *filter_exp;
    bpf_u_int32 net;
    // Open the device for packet capture
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 1;
    }
      
    filter_exp = filter_type(argc, argv);
    pcap_compile(handle, &fp, filter_exp, 0, net);
    pcap_setfilter(handle, &fp);
    // Capture packets indefinitely
    pcap_loop(handle, 4, packet_handler, NULL);
    // Close the handle
    pcap_close(handle);
    free(filter_exp);
    
    return 0;
}
