#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>



int main(int argc, char const *argv[])
{

int port = atoi(argv[2]);
//cast as pointer
struct sockaddr_in address;   //ADDRESS
memset(&address, 0, (sizeof(struct sockaddr_in)));
address.sin_port = htons(port);
address.sin_addr.s_addr = inet_addr(argv[1]);
address.sin_family = AF_INET;



int fileDesc = socket(AF_INET, SOCK_STREAM, 0);

	if(fileDesc < 0)
	{
		puts("Fail to connect.");
		return 0;

	}
	else
	{
		puts("Yay");


	int connectio =
 connect(fileDesc,(struct sockaddr*)&address, sizeof(address));
		
		if(connectio < 0)
			puts("Sad.");
		else{
			puts("Reading..");


			char buffer [3096] ={0};	
			//ERROR
			ssize_t lines = read(fileDesc, buffer, 300);
			printf("%s \n  %zd \n", buffer,lines);
		

			puts("M");
		}
	close(fileDesc);
	}
return 0;
}
