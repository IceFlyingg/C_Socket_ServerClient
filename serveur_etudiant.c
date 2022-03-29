#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>

#define PORT_SERVEUR 4148 // port d'écoute du serveur
#define IP_SERVEUR "127.0.0.1" //@IP serveur web
#define DEFAULT_BUFLEN 1500

int main(int argc, char* argv[])
{
	int num_socket, num_bind, num_listen, num_accept, num_send, num_recv;
	struct sockaddr_in serveur;
	char reponse0[] = "Nous avons recu votre requete numero 0";
	char reponse1[] = "Nous avons recu votre requete numero 1";
	char reponse[DEFAULT_BUFLEN];

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


	/******* Association entre l'adresse IP et le port d'écoute : bind() ***/
	serveur.sin_family=AF_INET;
	serveur.sin_port=htons(PORT_SERVEUR);
	serveur.sin_addr.s_addr = inet_addr(IP_SERVEUR);

	num_bind = bind(num_socket, (struct sockaddr_in*)&serveur, sizeof(serveur));

	if (num_bind == -1)
    {
        char* err = strerror(errno);
        printf("bind error : %s\n", err);
        return -1;
    }
    else
    {
        printf("Bind OK !\n");
    }


	/******* Configuration des demandes de connexions : listen () ***/
	num_listen = listen(num_socket, SOMAXCONN);

	if (num_listen == -1)
	{
		char* err = strerror(errno);
        printf("listen error : %s\n", err);
        return -1;
	}
	else
	{
		printf("Listen OK !\n");
	}


	while(1)
	{
		/******* Attente de nouvelle connexions : accept ()  **************/
		num_accept = accept(num_socket, NULL, NULL);

		if (num_accept == -1)
		{
			char* err = strerror(errno);
			printf("accept error : %s\n", err);
			return -1;
		}
		else
		{
			printf("Accept OK !\n");
		}


		/*****************  Réception de la requette : recv() **************/
		num_recv = recv(num_accept, reponse, DEFAULT_BUFLEN, 0);

		if ((num_recv == -1) || (num_recv == 0))
		{
			char* err = strerror(errno);
			printf("recv error : %s\n", err);
			return -1;
		}
		else
		{
			printf("Recv OK !\n");

			int reponseInt = atoi(reponse);

			/*****************  Envoi de la réponse : send() *******************/
			switch(reponseInt)
			{
				case 0:
					num_send = send(num_accept, reponse0, (int)strlen(reponse0), 0);

					if (num_send == -1)
					{
						char* err = strerror(errno);
						printf("recv error : %s\n", err);
						return -1;
					}
					else
					{
						printf("Send OK !\n");
					}
					break;
				case 1:
					num_send = send(num_accept, reponse1, (int)strlen(reponse1), 0);

					if (num_send == -1)
					{
						char* err = strerror(errno);
						printf("recv error : %s\n", err);
						return -1;
					}
					else
					{
						printf("Send OK !\n");
					}
					break;
			}
		}

		/*********  Fermeture connexion avec le client : close() ***********/
		close(num_accept);
	}

	/*****************  Fermeture socket : close() ******************/
	close(num_socket);
	return 0;
}
