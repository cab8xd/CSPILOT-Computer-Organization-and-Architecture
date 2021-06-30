//
//  chat.c
//  
//
//  Created by Christine Baca on 12/4/18.
//


/* Lab notes
 
 To work with files, you want to fopen them, access their contents, and then fclose them. The following components will help:
 
 Opening:
 
 FILE *inbox = fopen(filepath, "r"); returns NULL if filepath is not the path to a real file, and a pointer to a FILE structure opened in read-only mode otherwise.
 
 There is a special FILE * named stdin that is always open and reads from what the user types into the terminal window.
 
 FILE *outbox = fopen(filepath, "a"); returns NULL if filepath exists but you are not allowed to write to it, and a pointer to a FILE structure opened in append-only mode otherwise.
 
 There is a special FILE * named stdout that is always open and displays to the terminal window.
 
 Writing:
 
 fwrite(buffer, sizeof(char), 23, outbox); writes 23 char-sized values from buffer to FILE *outbox. Use it if you know how many bytes you want to write.
 
 fprintf(outbox, "%s: %d\n", "mst3k", 2501); writes mst3k 2501 and a newline to FILE *outbox. Like snprintf, this gives a lot of power with corresponding complexity.
 
 Reading:
 
 size_t got = fread(buffer, sizeof(char), 4096, inbox); reads up to 4096 char-sized values from FILE *inbox into buffer, and returns the number read (which is often less than 4096 if inbox did not have that many characters).
 
 This does not work well for stdin as the inbox parameter, as you don’t generally know how many characters the user will type. fgets is better for most user interactions.
 
 char *line = fgets(buffer, 4096, inbox); reads one line of text from FILE *inbox, or 4096 characters, whichever is less. It returns buffer on success and NULL on failure. The returned string includes the newline, as e.g. "mst3k\n"
 
 This works well for stdin as the inbox parameter, as users generally type one line at a time.
 
 Permissions, etc:
 
 chmod("/bigtemp/coa1/mst3k.chat", 0666); sets the permissions for mst3k.chat based on a big-vector of flags, specified in octal (hence the leading 0):
 
 The three octal digits are (in written order) the owner, group, and others permissions.
 If re-written in binary (e.g. change 07 to 1112), the bits mean may-read, may-write, and may-execute, in order.
 We want the files used in the chat to be writeable by any user, so 0666 is a reasonable permission set.
 truncate("/bigtemp/coa1/mst3k.chat", 0); truncates a file so its new size is 0 bytes. This is useful in re-setting a chat file after the user has read its contents.
 
 */




//------------------------------------------------

/* some referece to instructions:
 Repeatedly (i.e., in an infinite loop) use poll1 to pick either the connected socket (from the server’s accept or the client’s connect) or the standard input stream2 to read3 from. Use a 1-minute4 timeout for poll.
 If poll returns a positive number (i.e., it succeeded),
 If the standard input has revents including POLLIN, read from standard input and write what you read to the socket.
 If the socket has revents including POLLIN, read from the socket and write what you read to standard output5. You may assume no single message is more than 4096 bytes, to avoid needing to loop your read/write calls.
 */

//SERVER FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const char *msg = "Congratulations, you've successfully received a message from the server!\n";

int main() {
    //SERVER
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
    
    // we'll have one socket that waits for other sockets to connect to it
    // those other sockets will be the ones we used to communicate
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    
    // and we need to tell the OS that this socket will use the address created for it
    bind(listener, (struct sockaddr*)&ipOfServer , sizeof(ipOfServer));
    
    // wait for connections; if too many at once, suggest the OS queue up 20
    listen(listener , 20);
    
    system("host $HOSTNAME"); // display all this computer's IP addresses
    printf("The server is now listening on port %d\n", port); // and listening port
    
    for(;;) {
        // get a connection socket (this call will wait for one to connect)
        int connection = accept(listener, (struct sockaddr*)NULL, NULL);
        if (random()%2) { // half the time
            write(connection, msg, 40); // send half a message
            usleep(100000); // pause for 1/10 of a second
            write(connection, msg+40, strlen(msg+40)); // send the other half
        } else {
            write(connection, msg, strlen(msg)); // send a full message
        }
        close(connection); // and disconnect
    }
    
    // unreachable code, but still have polite code as good practice
    close(listener);
    return 0;
}



//------------------------------------------------

//CLIENT FILE
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



//------------------------------------------------
