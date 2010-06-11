/*
* The main function of this code segment is 
* to accept a dns request and apply with one IP encoded in packet. 
*
* Any problem, please contact Youyou Lu <luyouyou87@gmail.com>
*/

#include "dns.h"

int dns_client_init();
int dns_client_serve(int sockfd);
int dns_client_contruct_answer(char *pkt, int *pktlen);
int dns_client_domain2ip(const char* url, struct ip_t *ip);

int dns_client_init()
{
	int sockfd;
	struct sockaddr_in sin;
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(53);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("init error! \n");
		return sockfd;
	}

	if(bind(sockfd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		printf("bind error(%d): %s! \n", errno, strerror(errno));
		return errno;
	}

	dns_client_serve(sockfd);

	return 0;
}

int dns_client_serve(int sockfd)
{
	int n;
	char pkt[DNS_PACKET_LEN];
	char addrbuf[MAX_ADDR_LEN];
	int addrlen;

	for(;;)
	{
		addrlen = MAX_ADDR_LEN;
		if((n=recvfrom(sockfd, pkt, DNS_PACKET_LEN, 0, (struct sockaddr*)addrbuf, &addrlen)) < 0)
		{
			fprintf(stderr, "recvfrom error! \n");
			//return -1;
		}
		printf("recvd(%d/%d/%d): \n", n, strlen(pkt), sizeof(pkt));
		int idx = 0;	
		for(; idx<n; idx++)
		{
			if(idx%4 == 0)
				printf("\n");
			printf(" %x ", (uint8_t)pkt[idx]);
		}
		printf("\n");

		int pktlen = 0;
		dns_client_construct_answer(pkt, &pktlen);
		idx = 0;	
		for(; idx<pktlen; idx++)
		{
			if(idx%4 == 0)
				printf("\n");
			printf(" %x ", (uint8_t)pkt[idx]);
		}
		printf("\n pkt(%d/%d) to be send. \n", pktlen, strlen(pkt));		
		sendto(sockfd, pkt, pktlen, 0, (struct sockaddr*)addrbuf, addrlen);
	}

	return 0;
}

int dns_client_construct_answer(char *pkt, int *pktlen)
{
	char *p, *pdomain;
	char domainname[MAX_ADDR_LEN];

	//first, modify the header
	p = pkt+2;
	*(p++) = 0x81;
	*(p++) = 0x80;
	*(p++) = 0x00;
	*(p++) = 0x01;
	*(p++) = 0x00;
	*(p++) = 0x01;

	//next, get the question section
	p = pkt+12;
	pdomain = &domainname[0];
	while(((char)*p) != '\0')
	{
		int idx = (int)*p;
		for(; idx>0; idx--)
		{
			*pdomain = (char)*(++p);
			pdomain++;
		}
		++p;
		*(pdomain++) = '.';
	}
	*(--pdomain) = '\0';
	printf("domain name is : %s \n", domainname);

	//third, construct answer section
	p += 5;	//skip the '\0' bit and the 2*2bits type
	*(p++) = 0xc0;
	*(p++) = 0x0c;

	*(p++) = 0x00;
	*(p++) = 0x01;

	*(p++) = 0x00;
	*(p++) = 0x01;

	*(p++) = 0x00;
	*(p++) = 0x00;
	*(p++) = 0x00;
	*(p++) = 0x5f;

	*(p++) = 0x00;
	*(p++) = 0x04;

	struct ip_t ip;
	dns_client_domain2ip(domainname, &ip);
	printf("ip: %d.%d.%d.%d \n", ip.a, ip.b, ip.c, ip.d);
	*(p++) = ip.a;
	*(p++) = ip.b;
	*(p++) = ip.c;
	*(p++) = ip.d;
	
	*pktlen = p-pkt;
	return 0;	
}

int dns_client_domain2ip(const char* url, struct ip_t *ip)
{
    struct addrinfo *result = NULL;
    int ret;
    struct addrinfo addr;

    memset(&addr, 0 , sizeof(addr));
    addr.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(url, NULL, &addr, &result);
    if (!ret)
    {
        struct addrinfo *pCurr = result;
        printf("the \'%s\' ip is:\n", url);
        for (; pCurr; pCurr = pCurr->ai_next)
        {	
		printf("%s\n", inet_ntoa(((struct sockaddr_in*)(pCurr->ai_addr))->sin_addr));
		if(sscanf(inet_ntoa(((struct sockaddr_in*)(pCurr->ai_addr))->sin_addr), "%d.%d.%d.%d", &ip->a, &ip->b, &ip->c, &ip->d) == 4)
			break;
        }
    }

    return 0;
}

int main()
{
	dns_client_init();
	
	return 0;
}

