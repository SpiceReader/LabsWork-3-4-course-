#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <winsock2.h>  // Wincosk2.h должен быть 
// подключен раньше windows.h!
#include <windows.h>
#pragma warning( disable : 4996)

#define MY_PORT  666
		 // Порт, который слушает сервер

// макрос для печати количества активных
// пользователей 
#define PRINTNUSERS if (nclients)\
  printf("%d user on-line\n",nclients);\
  else printf("No User on line\n");

  // прототип функции, обслуживающий
  // подключившихся пользователей
DWORD WINAPI WorkWithClient(LPVOID client_socket);

// глобальная переменная – количество
// активных пользователей 
int nclients = 0;
bool check(char*);

int main(int argc, char* argv[])
{
	char buff[1024];    // Буфер для различных нужд

	printf("TCP SERVER DEMO\n");

	// -------------------------------------------
	// Шаг 1 - Инициализация Библиотеки Сокетов
	// -------------------------------------------
	// Т.к. возвращенная функцией информация
	// не используется ей передается указатель на
	// рабочий буфер, преобразуемый
	// к указателю  на структуру WSADATA.
	// Такой прием позволяет сэкономить одну
	// переменную, однако, буфер должен быть не менее
	// полкилобайта размером (структура WSADATA
	// занимает 400 байт)
	if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
	{
		// Ошибка!
		printf("Error WSAStartup %d\n",
			WSAGetLastError());
		return -1;
	}


	// -------------------------------------------
	// Шаг 2 - создание сокета
	// -------------------------------------------
	SOCKET mysocket;
	// AF_INET     - сокет Интернета
	// SOCK_STREAM  - потоковый сокет (с
	//      установкой соединения)
	// 0      - по умолчанию выбирается TCP протокол
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		// Ошибка!
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup();
		// Деиницилизация библиотеки Winsock
		return -1;
	}

	// -------------------------------------------
	// Шаг 3 связывание сокета с локальным адресом
	// -------------------------------------------
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT);
	// не забываем о сетевом порядке!!!
	local_addr.sin_addr.s_addr = 0;
	// сервер принимает подключения
	// на все IP-адреса

// вызываем bind для связывания
	if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
	{
		// Ошибка
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket);  // закрываем сокет!
		WSACleanup();
		return -1;
	}

	// -------------------------------------------
	// Шаг 4 ожидание подключений
	// -------------------------------------------
	// размер очереди – 0x100
	if (listen(mysocket, 0x100))
	{
		// Ошибка
		printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	printf("Waiting for connections\n");

	// -------------------------------------------
	// Шаг 5 извлекаем сообщение из очереди
	// -------------------------------------------
	SOCKET client_socket;    // сокет для клиента
	sockaddr_in client_addr;    // адрес клиента
			  // (заполняется системой)

	// функции accept необходимо передать размер
	// структуры
	int client_addr_size = sizeof(client_addr);

	// цикл извлечения запросов на подключение из
	// очереди
	while ((client_socket = accept(mysocket, (sockaddr *) &client_addr, &client_addr_size)))
	{
		nclients++;      // увеличиваем счетчик
				// подключившихся клиентов

		// пытаемся получить имя хоста
		//HOSTENT *hst;
		char hostname[1025];
		char servername[32];
		  
		//getnameinfo((struct sockaddr *) &local_addr, sizeof(local_addr), hostname, sizeof(hostname), servername, sizeof(servername), 0);
		
		//printf("host=%s, serv=%s\n", hostname, servername);
		// вывод сведений о клиенте
		//printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "", inet_ntop( AF_INET, &local_addr.sin_addr, buff, sizeof(buff)));
		HOSTENT *hst;
		hst = gethostbyaddr((char *) &client_addr.sin_addr.s_addr, 4, AF_INET);

		// вывод сведений о клиенте
		printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));

		PRINTNUSERS

			// Вызов нового потока для обслуживания клиента
			// Да, для этого рекомендуется использовать
			// _beginthreadex но, поскольку никаких вызов
			// функций стандартной Си библиотеки поток не
			// делает, можно обойтись и CreateThread
		DWORD thID;
		CreateThread(NULL, NULL, WorkWithClient, &client_socket, NULL, &thID);
	}
	return 0;
}

// Эта функция создается в отдельном потоке и
// обсуживает очередного подключившегося клиента
// независимо от остальных
DWORD WINAPI WorkWithClient(LPVOID client_socket)
{
	std::ofstream log;
	log.open("E:\\logs\\log.txt");
	log << " Hello message was send to client!!" << std::endl;
	log << " Client's socket: " << client_socket << std::endl;
	// цикл эхо-сервера: прием строки от клиента и
	// возвращение ее клиенту
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[20 * 1024];
#define sHELLO "Hello, Student!\r\n"

	// отправляем клиенту приветствие 
	send(my_sock, sHELLO, sizeof(sHELLO), 0);

	// цикл эхо-сервера: прием строки от клиента и
	// возвращение ее клиенту
	char hash[5], cod[10];
	int bytes_recv = 0, i = 0, limit = 0, control_sum = 0, control_sum2 = 0;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)
	{
		limit += bytes_recv;
		for (int i = 0; i < bytes_recv; i++)
		{
			control_sum += (int)buff[i];
		}
		if (check(&buff[0]))
		{
			send(my_sock, " Goodbye ", 10, 0);
			log << " Client's input 121. Disconnected. " << std::endl;
			break;
		}
		
		if (limit >= 9)
		{
			sprintf(hash, "%d", control_sum);
			send(my_sock, "  Server gets 10 symbols! \n", 28, 0);
			//send(my_sock, " The conrtol sum is: ", 22, 0);
			send(my_sock, hash, sizeof(hash), 0);
			log << " Client send 10 symbols! " << std::endl;
			log << " Hash =  " << hash << std::endl;
			limit = 0;
			control_sum = 0;
			//i = 0;
		}
		else {
			send(my_sock, &buff[0], bytes_recv, 0);
		}
	}

	// если мы здесь, то произошел выход из цикла по
	// причине возращения функцией recv ошибки –
	// соединение клиентом разорвано
	nclients--; // уменьшаем счетчик активных клиентов
	printf("-disconnect\n");
	PRINTNUSERS

		// закрываем сокет
	closesocket(my_sock);
	return 0;
}

bool check(char* str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (i + 2 <= strlen(str) && str[i] == '1' && str[i + 1] == '2' && str[i+2] == '1')
		{
			return true;
		}
	}
	return false;
}