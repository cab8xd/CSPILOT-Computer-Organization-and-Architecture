
//
//  schat.c
//
//
//  Created by Christine Baca on 12/4/18.

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>


int main(int argc, char const *argv[])
{
   
    if(argc != 1 && argc !=3)
    {
        puts("ERROR");
        return 0;
        
    }
    
if(argc == 1)
    {
        // start by getting a random port from the ephemeral port range
        srandom(getpid()); // random seed based on this process's OS-assigned ID
        int port = 0xc000 | (random()&0x3fff); // random element of 49152–65535
        
        // create an address structure: IPv4 protocol, anny IP address, on given port
        // note: htonl and htons are endian converters, essential for Internet communication
        struct sockaddr_in ipOfServer;
        memset(&ipOfServer, 0, sizeof(struct sockaddr_in));
        ipOfServer.sin_family = AF_INET;
        ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
        ipOfServer.sin_port = htons(port);
        printf("Port # is %d\n", port);
        
        int listener = socket(AF_INET, SOCK_STREAM, 0);
     
        bind(listener, (struct sockaddr*)&ipOfServer , sizeof(ipOfServer));
        
        // wait for connections; if too many at once, suggest the OS queue up 20
        listen(listener , 20);
        system("host $HOSTNAME"); // display all this computer's IP addresses
        printf("The server is now listening on port %d\n", port); // and listening port
        
        int connection = accept(listener, (struct sockaddr*)NULL, NULL);
        close(listener);
        
  //---SERVER LOOP
  
        struct pollfd fds[2];
        int fileDesc = connection; //The issue. Bad write. Resolved :D
        int timeout = 60000;
        int ret;
        int i;
        ssize_t bytes_read;
        

        fds[0].fd = fileDesc; //socket
        fds[1].fd = 0;//stdin
        
        fds[0].events = POLLIN;
        fds[1].events = POLLIN;
        
        for(;;){
            ret = poll(fds, 2, timeout); //use standard input

            if(ret > 0) {
                if(fds[1].revents & POLLIN)
                {
                    char buff[4096];
                    errno = 0;
                    ssize_t bytes_read = read(0,buff, 4096);
                    ssize_t bytes_written = write(fileDesc,buff,bytes_read);
                 
                }
                if(fds[0].revents & POLLIN){
                    char buff[4096];
                    errno = 0;
                    ssize_t bytes_read = read(fileDesc,buff,4096);
                    ssize_t bytes_written = write(1,buff,bytes_read);
                    
                }
                
                
            }
            
        }
        //---
    }
    //--------------------------------------------------------------------------

        printf("IP address: %s\n Port address: %s\n", argv[1], argv[2]);
        int port = atoi(argv[2]);
        
        struct sockaddr_in address;
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

            int connection =  connect(fileDesc,(struct sockaddr*)&address, sizeof(address));
            
            printf("Connection: %d", connection);
            //reading
            if(connection < 0)
            {
                puts("Int Connection Error.");              //ERROR
            }
            else
            {
          //---CLIENT LOOP
                //--
                
                    struct pollfd fds[2];
                    int timeout = 60000;
                    int ret;
                    int i;
                    ssize_t bytes_read;
                    
                
                    fds[0].fd = fileDesc; //socket
                    fds[1].fd = 0;//stdin
                
                    fds[0].events = POLLIN;
                    fds[1].events = POLLIN;
                    for(;;){
                    ret = poll(fds, 2, timeout); //use standard input
                    if(ret > 0) {
                        if(fds[1].revents & POLLIN)
                        {

                            char buff[4096];
                            errno = 0;
                            ssize_t bytes_read = read(0,buff, 4096);
                            ssize_t bytes_written = write(fileDesc,buff,bytes_read);
                        }
                        if(fds[0].revents & POLLIN){
                            char buff[4096];
                            ssize_t bytes_read = read(fileDesc,buff,4096);
                            ssize_t bytes_written = write(1,buff,bytes_read);
                            
                        }
                        
                        
                    }
          
            }
    //--
            
        }
    }
        
    
    return 0;
}


