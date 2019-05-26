#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#pragma warning(disable : 4996)

typedef struct msg_{
	int statusCode;
	int header;
	char data[1024];
} Message;

int main()
{
    int ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(1337);
    SockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(ClientSocket, (struct sockaddr *)&SockAddr, sizeof(SockAddr));

    char msg[1024];
    const char uname[] = "MyUName";
    Message message;
    strcpy(message.data, uname);
    message.header = strlen(uname);
    send(ClientSocket, &message, 4 + 4 + strlen(uname), 0); 
    recv(ClientSocket, &message.statusCode, 4, 0);
    if(message.statusCode != 200)
	return -1;
    recv(ClientSocket, &message.header, 4, 0);
    recv(ClientSocket, message.data, message.header, 0);

    while (1)
    {
        scanf("%s", msg);
	message.header = strlen(msg);
        send(ClientSocket, &message, 4 + 4 + strlen(msg), 0);
        recv(ClientSocket, &message.statusCode, 4, 0);
    	if(message.statusCode != 200)
		return -1;
   	recv(ClientSocket, &message.header, 4, 0);
    	recv(ClientSocket, message.data, message.header, 0);
        printf("%s\n", message.data);
    }

    return 0;
}

