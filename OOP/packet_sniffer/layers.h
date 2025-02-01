typedef struct IP IP_t;
typedef struct TCP TCP_t;
typedef struct UDP UDP_t;
typedef struct ICMP ICMP_t;
typedef struct APP APP_t;
			/* The network layer (IP protocol)*/
struct IP{
	struct iphdr *ip;
	void (*IP_Header)(IP_t *ip_obj);
};

IP_t *ip_construct(const u_char *packet);

void print_ip_header(struct IP *ip_obj);

void ip_destruct(IP_t *obj);
			/* The transport layer */
			
/* The TCP protocol */
struct TCP{
	struct tcphdr *tcp;
	void (*TCP_Header)(TCP_t *tcp_obj);
};

TCP_t *tcp_construct(const u_char *packet, IP_t *ip_obj);

void print_tcp_header(struct TCP *tcp_obj);

void tcp_destruct(TCP_t *obj);

/* The UDP protocol */
struct UDP{
	struct udphdr *udp;
	void (*UDP_Header)(UDP_t *udp_obj);
};

UDP_t *udp_construct(const u_char *packet, IP_t *ip_obj);

void print_udp_header(struct UDP *udp_obj);

void udp_destruct(UDP_t *obj);

/* The ICMP protocol */
struct ICMP{
	struct icmphdr *icmp;
	void (*ICMP_Header)(ICMP_t *icmp_obj);
};

ICMP_t *icmp_construct(const u_char *packet, IP_t *ip_obj);

void print_icmp_header(struct ICMP *icmp_obj);

void icmp_destruct(ICMP_t *obj);


			/* The application layer */
struct APP{
	void (*APP_Proto)(TCP_t *tcp_obj);
};

APP_t *app_construct();

void print_app_protocol(TCP_t *tcp_obj);

void app_destruct(APP_t *obj);

char* filter_type(int argc, char **argv);
