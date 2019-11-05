/*
*****************************************************************************************
*
*        		===============================================
*           		Rapid Rescuer (RR) Theme (eYRC 2019-20)
*        		===============================================
*
*  This script is to implement Task 1B of Rapid Rescuer (RR) Theme (eYRC 2019-20).
*
*  This software is made available on an "AS IS WHERE IS BASIS".
*  Licensee/end user indemnifies and will keep e-Yantra indemnified from
*  any and all claim(s) that emanate from the use of the Software or
*  breach of the terms of this agreement.
*
*  e-Yantra - An MHRD project under National Mission on Education using ICT (NMEICT)
*
*****************************************************************************************
*/

/*
* Team ID:			[ 5151 ]
* Author List:		[ Rohan Mehta, Arnav Saha ]
* Filename:			robot-server.c
* Functions:		socket_create, receive_from_send_to_client
* 					[ Comma separated list of functions in this file ]
* Global variables:	SERVER_PORT, RX_BUFFER_SIZE, TX_BUFFER_SIZE, MAXCHAR,
* 					dest_addr, source_addr, rx_buffer, tx_buffer,
* 					ipv4_addr_str, ipv4_addr_str_client, listen_sock, line_data, input_fp, output_fp
* 					[ List of global variables defined in this file ]
*/

// Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

// Constants defined
#define SERVER_PORT 3333
#define RX_BUFFER_SIZE 1024
#define TX_BUFFER_SIZE 1024

#define MAXCHAR 1000 // max characters to read from txt file

// Global variables
struct sockaddr_in dest_addr;		// SERVER
struct sockaddr_in source_addr; // CLIENT

char rx_buffer[RX_BUFFER_SIZE]; // buffer to store data from client
char tx_buffer[RX_BUFFER_SIZE]; // buffer to store data to be sent to client

char ipv4_addr_str[128];				// buffer to store IPv4 addresses as string
char ipv4_addr_str_client[128]; // buffer to store IPv4 addresses as string

int listen_sock;

char line_data[MAXCHAR];

FILE *input_fp, *output_fp;

/*
* Function Name:	socket_create
* Inputs:			dest_addr [ structure type for destination address ]
* 					source_addr [ structure type for source address ]
* Outputs: 			my_sock [ socket value, if connection is properly created ]
* Purpose: 			the function creates the socket connection with the server
* Example call: 	int sock = socket_create(dest_addr, source_addr);
*/
int socket_create(struct sockaddr_in dest_addr, struct sockaddr_in source_addr)
{

	int addr_family;
	int ip_protocol;

	//dest_addr is server address and source_addr is client
	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;
	bzero(&dest_addr.sin_zero, 0);

	int my_sock, cli, len = sizeof(struct sockaddr_in);

	printf("[DEBUG] Self IP = %s\n", inet_ntoa(dest_addr.sin_addr));
	//creating socket
	if ((my_sock = socket(addr_family, SOCK_STREAM, 0)) == -1)
	{
		perror("socket creater error: ");
		exit(-1);
	}
	printf("[DEBUG] Socket created\n");

	//binding socket
	if ((bind(my_sock, (struct sockaddr *)&dest_addr, len)) == -1)
	{
		perror("bind: ");
		exit(-1);
	}
	printf("[DEBUG] Socket bound, port %d\n", SERVER_PORT);

	//listening socket
	if ((listen(my_sock, 5)) == -1)
	{
		perror("listen: ");
		exit(-1);
	}
	printf("[DEBUG] Socket listening\n");

	//accepting connections
	if ((cli = accept(my_sock, (struct sockaddr *)&source_addr, &len)) == -1)
	{
		perror("accept: ");
		exit(-1);
	}
	printf("[DEBUG] Socket accepted\n");
	return cli;
}

/*
* Function Name:	receive_from_send_to_client
* Inputs:			sock [ socket value, if connection is properly created ]
* Outputs: 			None
* Purpose: 			the function receives the data from server and updates the 'rx_buffer'
*					variable with it, sends the obstacle position based on obstacle_pos.txt
*					file and sends this information to the client in the provided format.
* Example call: 	receive_from_send_to_client(sock);
*/
int receive_from_send_to_client(int cli)
{

	int sent_bytes, recv_bytes, line_data_read = 0, recv_file_num, current_file_num = 0, no_obstacle = 0;
	//reset the buffer for fresh data
	memset(rx_buffer, 0, RX_BUFFER_SIZE);

	while (1)
	{
		recv_bytes = recv(cli, rx_buffer, sizeof(rx_buffer) - 1, 0);
		if (recv_bytes > 0)
		{
			printf("\n------------------------------------------------\n");
			printf("[DEBUG] Received %d bytes from %s\n", recv_bytes, inet_ntoa(source_addr.sin_addr));
			printf("[DEBUG] Data Received =  %s\n", rx_buffer);

			recv_file_num = (int)rx_buffer[0] - 48;

			//moving the input file pointer to read from beginning
			fseek(input_fp, 0, SEEK_SET);
			while (fgets(line_data, MAXCHAR, input_fp) != NULL)
			{
				current_file_num = (int)line_data[0] - 48;
				if (current_file_num != recv_file_num)
				{
					no_obstacle = 1;
				}
				else
				{
					no_obstacle = 0;
					break;
				}
			}

			//condition when no obstacles are left to send
			if (line_data_read == strlen(line_data) - 1 || no_obstacle == 1)
			{
				//end of obstacle marker for client
				char end[3] = "@$@";
				sent_bytes = send(cli, end, strlen(end), 0);
				printf("[DEBUG] Transmitted %d bytes: %s\n", sent_bytes, end);
				//break from the outer while loop
				break;
				//final path will be written to file by main()
			}
			//save the received data from client in the file
			fputs(rx_buffer, output_fp);
			fputs("\n", output_fp);
			//clear buffer for further use
			memset(rx_buffer, 0, RX_BUFFER_SIZE);

			//prepare obstacles
			char obs[10], obs_i = 0;
			for (int i = line_data_read; i <= strlen(line_data); i++)
			{
				if (line_data[i] == '(')
				{
					obs[obs_i++] = '@';
					for (int j = i;; j++)
					{
						if (line_data[j] == '\r')
						{
							continue;
						}
						if (line_data[j] == ';' || line_data[j] == '\n')
						{

							// if (line_data[j] == '\0')
							// {
							// 	line_data_read = j - 1;
							// }
							// else
							// {
							line_data_read = j;
							// }

							break;
						}
						obs[obs_i++] = line_data[j];
						line_data_read = j;
					}
					obs[obs_i] = '@';
					break;
				}
			}
			//sending obstacle
			sent_bytes = send(cli, obs, strlen(obs), 0);
			printf("[DEBUG] Transmitted %d bytes: %s\n", sent_bytes, obs);
			memset(obs, 0, 10);
		}
	}
	//when last maze is done
	//we can't change the main(), so closing all file pointers and exiting here
	if (recv_file_num == 4)
	{
		fputs(rx_buffer, output_fp);
		fputs("\n", output_fp);
		fclose(input_fp);
		fclose(output_fp);
		exit(0);
	}
	return 0;
}

/*
* Function Name:	main()
* Inputs:			None
* Outputs: 			None
* Purpose: 			the function solves Task 1B problem statement by making call to
* 					functions socket_create() and receive_from_send_to_client()
*/
int main()
{

	char *input_file_name = "obstacle_pos.txt";
	char *output_file_name = "data_from_client.txt";

	// Create socket and accept connection from client
	int sock = socket_create(dest_addr, source_addr);

	input_fp = fopen(input_file_name, "r");

	if (input_fp == NULL)
	{
		printf("Could not open file %s\n", input_file_name);
		return 1;
	}

	fgets(line_data, MAXCHAR, input_fp);

	output_fp = fopen(output_file_name, "w");

	if (output_fp == NULL)
	{
		printf("Could not open file %s\n", output_file_name);
		return 1;
	}

	while (1)
	{

		// Receive and send data from client and get the new shortest path
		receive_from_send_to_client(sock);

		// NOTE: YOU ARE NOT ALLOWED TO MAKE ANY CHANGE HERE
		fputs(rx_buffer, output_fp);
		fputs("\n", output_fp);
	}

	return 0;
}
