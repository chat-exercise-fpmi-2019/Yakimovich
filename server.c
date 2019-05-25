#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#pragma warning(disable:4996)

typedef struct mapUsername{
	char uname[100];
	int socket;
} UserNameSocket;

UserNameSocket userNameSockets[100];
int userCount = 0;


int findBySocket(int socket)
{
	for(int i = 0; i < 100; i++)
	{
		if(userNameSockets[i].socket == socket)
			return i;
	}
	return 0;
}

int eq(char name1[100], char name2[100])
{
	for(int i = 0; i < 100; i++)
	{
		if(name1[i] != name2[i])
			return 0;	
	}
	return 1;
}

int findByName(char name[100])
{
	for(int i = 0; i < 100; i++)
	{
		if(eq(userNameSockets[i].uname, name))
			return i;
	}
	return 0;
}

int main()
{
    int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(1337);
    SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));

    listen(MasterSocket, SOMAXCONN);

    printf("Waiting 4 a client...\n");
    int ActiveSocket;
    fd_set socketsSet, recievedSet;
    int max = MasterSocket;

    FD_ZERO(&socketsSet);
    FD_ZERO(&recievedSet);
    FD_SET(MasterSocket, &socketsSet);

    while (1)
    {
        recievedSet = socketsSet;

        select(max + 1, &recievedSet, NULL, NULL, NULL);

        for (ActiveSocket = 0; ActiveSocket <= max; ActiveSocket++)
        {
            if (FD_ISSET(ActiveSocket, &recievedSet))
            {
                if (ActiveSocket != MasterSocket)
                {
                    char Buffer[1025];
		    char message[1024] = "|MESG[";
		    char Reciever[100];
		    int offset;
		    int i;
                    size_t msg_size = recv(ActiveSocket, Buffer, 1024, 0);
		    int pos;
                    Buffer[msg_size] = 0;
                    printf("Client #%d send msg: %s\n", ActiveSocket, Buffer);
		    switch(Buffer[1])
		    {
			case 'U':
				pos = findBySocket(ActiveSocket);
				for(i = 6; i < 106 && Buffer[i] != '|'; i++)
					userNameSockets[pos].uname[i-6] = Buffer[i];
				printf("Name: %s \n", userNameSockets[pos].uname);
		    		send(ActiveSocket, Buffer, msg_size, 0);
				break;
			case 'P':
				pos = findBySocket(ActiveSocket);
				for(i = 6; i < 106 && Buffer[i] != '|'; i++)
					message[i] = Buffer[i];
				for(i = 6; Buffer[i] != ']'; i++)
					Reciever[i-6] = Buffer[i];
				printf("Reciever: %s\n", Reciever);
				printf("RecieverID: %d\n", findByName(Reciever));
                    		send( userNameSockets[findByName(Reciever)].socket , message, msg_size, 0);
				break;
			case 'B':
				pos = findBySocket(ActiveSocket);
				i = 6;
				offset = i;
				printf("Sender: %s\n", userNameSockets[pos].uname);
				for(; userNameSockets[pos].uname[i - offset] != '\0'; i++)
					message[i] = userNameSockets[pos].uname[i - offset];
				message[i] = ']';
				i++;
				message[i] = '[';
				i++;
				offset = i;
				for(i = 6; i < 1024 && Buffer[i] != '|'; i++)
					message[offset + i - 6] = Buffer[i];
				offset = offset + i - 6;
				for(i = 0; i < 100; i++)
                    			send( userNameSockets[i].socket , message, offset, 0);
				break;
		    }
                    printf("Client #%d send msg: %s\n", ActiveSocket, Buffer);
                }
                else
                {
                    ActiveSocket = accept(MasterSocket, 0, 0);
                    FD_SET(ActiveSocket, &socketsSet);
                    if (ActiveSocket > max)
                        max = ActiveSocket;
		    userNameSockets[userCount].socket = ActiveSocket;
		    userCount++;
                    printf("Client #%d connected!\n", ActiveSocket);
                }
            }
        }


    }

    return 0;
}
