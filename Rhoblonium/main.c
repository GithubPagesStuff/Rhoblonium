#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif
#include <netdb.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <errno.h>
#include "LocalDefines.h"
#include <cURL/cURL.h>
struct rcc {
	in_addr_t rcc_ip;
	uint32_t rcc_port;
	char jobid[1024]; //
	int players;
	int placeid;
	int timeremaining;
	
};
struct rcc* rcc_servs[32]; //32 ips should be enough
int get_req(in_addr_t adr, char*pf) {
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	
	
	
	if (curl) {
		struct in_addr eiin;
		eiin.s_addr = adr;
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_URL, strcat(inet_ntoa(eiin), ":64989/"));
		struct curl_slist *hds = NULL;
		hds = curl_slist_append(hds, "Content-Type: application/xml");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pf);
		curl_easy_setopt(curl, CURLOPT_HEADER, hds);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(hds);
	}
	return 0;
}

/*int get__request(in_addr_t adr, char*fn) { old broken thing
 int socketfd;
 int len;
 char *t2=malloc(20000);
 strcpy(t2, "POST / HTTP/1.1\r\nContent-Type: application/xml\r\n\r\n\n");
 strcat(t2, fn);
 strcat(t2, "\n\n");
 char *temp = strdup(t2);
 printf(temp);
 char request[1024];
 char buffer[1024];
 const char *eeee;
 struct sockaddr_in s_in;
 struct protoent *pent;
 
 static const char *ipaddr = "3.17.144.232";
 pent=getprotobyname("tcp");
 s_in.sin_addr.s_addr = adr;
 s_in.sin_family = AF_INET;
 s_in.sin_port = htons(64989);
 struct timeval timeout;
 timeout.tv_sec=1;
 timeout.tv_usec=0;
 struct in_addr iiadrn;
 iiadrn.s_addr=adr;
 socketfd = socket(AF_INET, SOCK_STREAM, pent->p_proto);
 int flags = fcntl(socketfd, F_GETFL,0) | O_NDELAY;
 //fcntl(socketfd, F_SETFL, flags);
 //setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
 int con = connect(socketfd, (struct sockaddr*)&s_in, sizeof(s_in));
 int stat;
 fd_set set;
 FD_ZERO(&set);
 FD_SET(socketfd, &set);
 stat = select(socketfd+1, NULL, &set, NULL, &timeout);
 char *res = inet_ntoa(iiadrn);
 //current_timestamp(res);
 
 int e4=1;
 while(1) {
 stat = select(socketfd+1, NULL, &set, NULL, &timeout);
 int eern = errno;
 if (eern == 32) {
 return -1; // handle sigpipes.
 }
 
 if (stat > 0) {
 char *rees = inet_ntoa(iiadrn);
 int ernn = errno;
 //printf("here is the real nomad thingy with working connection!!! at %s \n", rees);
 flags &= (~O_NONBLOCK);
 int ern = errno;
 //fcntl(socketfd, F_SETFL, flags);
 //connect(socketfd, (struct sockaddr*)&s_in, sizeof(s_in));
 int byten = 0;
 while (byten < strlen(temp)) {
 int pb = write(socketfd, temp+byten, strlen(temp)-byten);
 int pbe = errno;
 if (pbe == 32) {
 break;
 return -1;
 }
 byten=byten+pb;
 }
 byten = read(socketfd,buffer, 1024);
 int readerrno = errno;
 write(STDOUT_FILENO, buffer, byten);
 printf("%s @ %s \n", buffer, rees);
 return(-1);
 //write(STDOUT_FILENO, strstr(buffer, (const char*)"nomad"), 10);
 //printf("%s", strstr(buffer, (const char*)"nomad"));
 if (strstr(buffer, (const char*)"nomad") != 0x0) {
 printf("nomad detected at %s", rees);
 int ei = close(socketfd);
 stat=-1;
 return 0;
 }
 
 } else if(stat==0){
 char* rees = inet_ntoa(iiadrn);
 //current_timestamp(rees);
 //printf("fail at %s \n", rees);
 close(socketfd);
 return -1;
 }
 }
 
 close(socketfd);
 
 return -1;
 }*/
int renewLease(struct rcc e) {
	char fn[10000];
	snprintf(fn, 10000, "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http://roblox.com/\">\r\n<soapenv:Header/>\r\n<soapenv:Body>\r\n<ns1:GetExpiration>\r\n<ns1:jobID>Main</ns1:jobID>\r\n</ns1:GetExpiration>\r\n</soapenv:Body>\r\n</soapenv:Envelope>\r\n\r\n", e.placeid);
	get_req(e.rcc_ip, &fn);
	return 0;
};

int getPlayersAmount(struct rcc e) {
	char fn[10000];
	snprintf(fn, 10000, "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http://roblox.com/\">\r\n<soapenv:Header/>\r\n<soapenv:Body>\r\n<ns1:ExecuteEx>\r\n<ns1:jobID>%s</ns1:jobID>\r\n<ns1:script>\r\n<ns1:Name>SOAP</ns1:Name>\r\n<ns1:script>{\r\n\"Type\": \"run\",\r\n\"Mode\": \"ExecuteScript\",\r\n}\r\n</ns1:arguments>\r\n</ns1:script>\r\n<!--Optional:-->\r\n<ns1:arguments>\r\n<!--Zero or more repetitions:-->\r\n<ns1:LuaValue>\r\n<ns1:type>0</ns1:type>\r\n<!--Optional:-->\r\n<ns1:value>print(\"hi\")</ns1:value>\r\n<!--Optional:-->\r\n<ns1:table/>\r\n</ns1:LuaValue>\r\n</ns1:arguments>\r\n</ns1:script>\r\n</ns1:ExecuteEx>\r\n</soapenv:Body>\r\n</soapenv:Envelope>", e.jobid);
	get_req(e.rcc_ip, &fn);
	
	
	
	return 15;
};
void *rccthread(void *rcie) {
	//start the job
	struct rcc rccth = *(rcc_servs[*(int*)rcie]);
	char fn[10000];
	
	
	snprintf(fn, 10000, "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http://roblox.com/\">\r\n<soapenv:Header/>\r\n<soapenv:Body>\r\n<ns1:OpenJobEx>\r\n<ns1:job>\r\n<ns1:id>%s</ns1:id>\r\n<ns1:expirationInSeconds>100</ns1:expirationInSeconds>\r\n<ns1:category>1</ns1:category>\r\n<ns1:cores>1</ns1:cores>\r\n</ns1:job>\r\n<!--Optional:-->\r\n<ns1:script>\r\n<!--Optional:-->\r\n<ns1:name>Main</ns1:name>\r\n<!--Optional:-->\r\n<ns1:script>{\r\n\"Type\": \"Place\",\r\n\"Mode\": \"GameServer\",\r\n\"Settings\": {\r\n\"BaseUrl\": \"roblox.com\",\r\n\"PlaceFetchUrl\": \"https://assetdelivery.roblox.com/v1/asset?id=%d\",\r\n\"PlaceId\": %d,\r\n\"UniverseId\": 13058,\r\n\"MatchmakingContextId\": 1,\r\n\"CreatorId\": 1,\r\n\"CreatorType\": \"User\",\r\n\"PlaceVersion\": 0,\r\n\"GameId\": \"5cac6fa4-cf2a-4f4c-bcb0-e28c76e589b9\",\r\n\"MachineAddress\": \"128.116.75.26\",\r\n\"GsmInterval\": 1000,\r\n\"MaxPlayers\": 10,\r\n\"MaxGameInstances\": 20,\r\n\"ApiKey\": \"258EAFA5-E914-47DA-95CA-C5AB0DC85B11\",\r\n\"PreferredPlayerCapacity\": 10,\r\n\"DatacenterId\": 299\r\n}\r\n}</ns1:script>\r\n<!--Optional:-->\r\n<ns1:arguments>\r\n<!--Zero or more repetitions:-->\r\n<ns1:LuaValue>\r\n<ns1:type>0</ns1:type>\r\n<!--Optional:-->\r\n<ns1:value>print(\"hi\")</ns1:value>\r\n<!--Optional:-->\r\n<ns1:table/>\r\n</ns1:LuaValue>\r\n</ns1:arguments>\r\n</ns1:script>\r\n</ns1:OpenJobEx>\r\n</soapenv:Body>\r\n</soapenv:Envelope>\r\n\r\n",rccth.jobid, rccth.placeid, rccth.placeid);
	get_req(rccth.rcc_ip, &fn);
	int plrAmount = 1;
	sleep(5);
	getPlayersAmount(rccth);
	return 0;
};

int main(int argc, const char * argv[]) {
	// insert code here...
	struct rcc e;
	e.rcc_port=1234;
	e.rcc_ip=inet_addr(IPADDR);
	e.placeid=0;
	rcc_servs[0]=&e;
	int socketfd;
	int len;
	char buffer[1024];
	const char *eeee;
	struct sockaddr_in s_in;
	struct sockaddr_in c_in;
	socklen_t clilen;
	struct protoent *pent;
	int nsfd;
	pent=getprotobyname("tcp");
	s_in.sin_addr.s_addr = INADDR_ANY;
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(420);
	struct timeval timeout;
	timeout.tv_sec=5;
	timeout.tv_usec=0;
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(socketfd, (struct sockaddr *)&s_in, sizeof(s_in));
	listen(socketfd, 5);
	clilen=sizeof(c_in);
	const char*IIe[1024];
	while (1) {
		nsfd = accept(socketfd, (struct sockaddr *) &c_in, &clilen);
		int n = read(nsfd, buffer, 1023);
		char *bf = strdup((char*)&buffer);
		int e = errno;
		char *tok;
		int tokamt = 0;
		while( (tok = strtok_r(bf,"\r\n", &bf)) != NULL ) {
			IIe[tokamt]=tok;
			tokamt++;
			printf("%s\n",tok);
			
		}
		if (strcmp("RunPlace", IIe[0]) == 0) {
			char *eptr;
			printf("RUN PLAC");
			int plid = strtol(IIe[1], &eptr, 10);
			struct rcc newrcc;
			for (int ei=0; ei<=32; ei++) {
				if (rcc_servs[ei]== NULL) {
					break;
					
				}
				if (rcc_servs[ei]->placeid == 0) {
					rcc_servs[ei]->placeid=plid;
					strcpy(rcc_servs[ei]->jobid, IIe[2]);
					rcc_servs[ei]->players=1;
					pthread_t pt;
					int *eee = NULL;
					eee=ei;
					pthread_create(&pt, NULL, rccthread, (void*)&eee);
					pthread_detach(pt);
					break;
					
				}
			}
		}
	}
	//setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
	return 0;
}

