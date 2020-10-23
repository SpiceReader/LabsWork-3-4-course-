#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <fstream>
#include <ctime>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS



#define PORT 666
#define SERVERADDR "127.0.0.1"

int main(int argc, char* argv[])
{
	char buff[50];
	int s, sfd;

	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);                         // текущая дата, выраженная в секундах
	timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме
	std::cout << asctime(timeinfo);

	printf("TCP DEMO CLIENT\n");

	// Шаг 1 - инициализация библиотеки Winsock
	if (WSAStartup(0x202, (WSADATA *)&buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 2 - создание сокета
	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	/*char ip[32];
	char port[5];
	printf(" For connecting to server write - connect (serverip) (serverport) !!\n");
	printf(" connect ");
	scanf("%s ", &ip);
	scanf("%s", &port);*/
	
	// Шаг 3 - установка соединения

	// заполнение структуры sockaddr_in
	// указание адреса и порта сервера
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);

	//dest_addr.sin_addr.s_addr = 

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	HOSTENT *hst;
	// преобразование IP адреса из символьного в
	// сетевой формат
	//int r = getaddrinfo("127.0.0.1", "8000", &hints, &result);
	void* ipstr[128];
	char hostname[32];
	char servername[32];
	char node[32];

	if (inet_addr(SERVERADDR) != INADDR_NONE) {
		std::cout << inet_addr(SERVERADDR) << std::endl;
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	}
	else {
		if (hst = gethostbyname(SERVERADDR)) {
			// hst->h_addr_list содержит не массив адресов,
			// а массив указателей на адреса

			((unsigned long *)&dest_addr.sin_addr)[0] = ((unsigned long **)hst->h_addr_list)[0][0];
		}
		else
		{
			printf("Invalid address %s\n", SERVERADDR);
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
	}
	
	// адрес сервера получен – пытаемся установить
	// соединение 
	if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		printf("Connect error %d\n", WSAGetLastError());
		return -1;
	}

	printf("Connection to %s is established\n\
    Type quit for quit\n\n", SERVERADDR);
	std::ofstream log;
	log.open("E:\\logs\\logClient.txt");
	// Шаг 4 - чтение и передача сообщений
	int nsize;
	while ((nsize = recv(my_sock, &buff[0],sizeof(buff) - 1, 0))!= SOCKET_ERROR)
	{
		// ставим завершающий ноль в конце строки 
		buff[nsize] = 0;
		//printf("Size %d", nsize);

		log << " Message was accepted from server!!" << &buff[0] << "Time: ";
		log << asctime(timeinfo) << std::endl;
		// выводим на экран 
		printf("S=>C:%s", buff);

		// читаем пользовательский ввод с клавиатуры
		printf("S<=C:"); 
		fgets(&buff[0], sizeof(buff) - 1, stdin);
		//printf("len %d", strlen(buff));
		log << " The message, which was sended to server!! " << &buff[0] << "Time: ";
		log << asctime(timeinfo) << std::endl;
		// проверка на "quit"
		if (!strcmp(&buff[0], "disconnect 127.0.0.1 666\n"))
		{
			// Корректный выход
			printf("Exit...");
			closesocket(my_sock);
			WSACleanup();
			return 0;
		}
		if (!strcmp(&buff[0], "get\n"))
		{
			send(my_sock, &buff[0], 0, 0);
			// Корректный выход
			printf("\n");
			printf("Hash - ");
		}
		else {
			nsize = strlen(buff);
			// передаем строку клиента серверу
			send(my_sock, &buff[0], nsize, 0);
		}
	}


	printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	log.close();
	WSACleanup();
	return -1;
}
