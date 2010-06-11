/*
* ipfilter.c
* To filer the IP from specific range.
*
* Any problem, please contact Youyou Lu <luyouyou87@gmail.com>
*
*/

#include "dns.h"

/*
* return: 0 -> pass ; -1 -> fail
* filter: 127.0.0.1; 10.0.*; 192.168.*; 166.111.*; 59.66.*
*/
int ipfilter(struct in_addr *addr)
{
	//in_addr_t local = inet_addr("127.0.0.1");
	uint32_t addrin = (uint32_t)addr->s_addr;
	//printf("%i \n", addrin);
	uint32_t lh = htonl(127<<24|1);
	uint32_t lan1 = htonl(10<<24);
	uint32_t lan2 = htonl(192<<24|168<<16);
	uint32_t tsinghua1 = htonl(166<<24|111<<16);
	uint32_t tsinghua2 = htonl(59<<24|66<<16);

	printf("%d vs %d", addrin, lh);
	if(addrin == lh)
	{
		printf("ipfilter: local machine(127.0.0.1). \n");
		return 0;
	}
	if((addrin & 0xFF00) == (lan1 & 0xFF00))
	{
		printf("ipfilter: lan1(10.0.*). \n");
		return 0;
	}
	if((addrin & 0xFF00) == (lan2 & 0xFF00))
	{
		printf("ipfilter: lan2(192.168.*) \n");
		return 0;
	}
	if((addrin & 0xFF00) == (tsinghua1 & 0xFF00))
	{
		printf("ipfilter: tsinghua1(166.111.*) \n");
		return 0;
	}
	if((addrin & 0xFF00) == (tsinghua2 & 0xFF00))
	{
		printf("ipfilter: tsinghua2(59.66.*) \n");
		return 0;
	}

	return -1;
}

/*
//for test only
int main()
{
	struct sockaddr_in tmp;
	//tmp.sin_addr.s_addr = inet_addr(127.0.0.1);
//	tmp.sin_addr.s_addr = htonl(127<<24|1);
//tmp.sin_addr.s_addr = htonl(10<<24|1);
//tmp.sin_addr.s_addr = htonl(192<<24|168<<16|1);
//tmp.sin_addr.s_addr = htonl(166<<24|111<16|1);
tmp.sin_addr.s_addr = htonl(59<<24|66<16|1);
	printf("%d \n", tmp.sin_addr.s_addr);
	//struct in_addr_t testlh = inetaddr(127.0.0.1);
	printf("test loca host: %d \n", ipfilter(&tmp.sin_addr));
	return 0;
}
*/
