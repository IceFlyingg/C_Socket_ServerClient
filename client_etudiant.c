#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>

#define DEFAULT_BUFLEN 1500

int main(int argc, char* argv[])
{
    int num_socket, num_connect, num_send, num_recv;
    struct sockaddr_in serveur;
    //int port = 80;
    //char IP[16] = "127.0.0.1"; // @ Ip du serveur
    //char requette[] = "GET / HTTP/1.1\r\nHost: 192.168.176.2\r\n\r\n"; // Requette HTTP
    int port = 4148;
    char IP[16] = "127.0.0.1";
    char requette[] = "blblb";
    char reponse[DEFAULT_BUFLEN]; // Buffer pour la reception

    /******* Creation de la socket SOCK_STREAM : socket() ***************/
    num_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (num_socket == -1)
    {
        char* err = strerror(errno);
        printf("socket error : %s\n", err);
    }
    else
    {
        printf("Socket creation OK !\n");
    }


    /******* Connexion avec un serveur distant : connect() ***************/
    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(port);
    serveur.sin_addr.s_addr = inet_addr(IP);

    num_connect = connect(num_socket, (const struct sockaddr*)&serveur, sizeof(serveur));

    if (num_connect == -1)
    {
        char* err = strerror(errno);
        printf("connect error : %s\n", err);
        return -1;
    }
    else
    {
        printf("Connect OK !\n");
    }

    /*****************  Requette : send() ****************************/
    num_send = send(num_socket, requette, (int)strlen(requette), 0);

    if (num_send == -1)
    {
        char* err = strerror(errno);
        printf("send error : %s\n", err);
        return -1;
    }
    else
    {
        printf("Send OK !\n");
    }


    /*****************  Réponse : recv () ****************************/
    num_recv = recv(num_socket, reponse, DEFAULT_BUFLEN, 0);

    if ((num_recv == -1) || (num_recv == 0))
    {
        char* err = strerror(errno);
        printf("recv error : %s\n", err);
        return -1;
    }
    else
    {
        printf("%s", reponse);
    }


    /*****************  Fermeture socket : close() ******************/
    close(num_socket);

    return 0;	
}
