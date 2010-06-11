#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DNS_PACKET_LEN 2048 
#define MAX_ADDR_LEN 256

uint16_t tid;

enum dns_query_type {DNS_A_RECORD = 0x01, DNS_MX_RECORD = 0x0f};

struct dns_header
{
	uint16_t id;
	uint16_t flags;
	uint16_t num_questions;
	uint16_t num_answers;
	uint16_t num_auths;
	uint16_t num_addtions;
};

/*
struct dnsHeader
{
	unsigned id :16;	//query identification number

	unsigned rd: 1;		//recursion desired
	unsigned tc: 1;		//truncated message
	unsigned aa: 1;		//authoritive answer
	unsigned opcode: 4;	//option code
	unsigned qr: 1;		//response flag

	unsigned rcode :4;	//response code
	unsigned cd: 1;		//checking disabled by resolver
	unsigned ad: 1;		//authentic data from named
	unsigned unused :1;	//unused
	unsigned ra: 1;		//recursion available

	unsigned question_count :16;	//number of question entries
	unsigned answer_count :16;	//number of answer entries
	unsigned authority_count :16;	//number of authority entries
	unsigned addtion_count :16;	//number of additional entries
};
*/

struct resolv_sock
{
	int sock;
	struct sockaddr_in sa;
};

struct ip_t
{
	int a;
	int b;
	int c;
	int d;
};

