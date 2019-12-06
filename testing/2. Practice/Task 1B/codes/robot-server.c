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
* Team ID:			[ Team-ID ]
* Author List:		[ Names of team members worked on this file separated by Comma: Name1, Name2, ... ]
* Filename:			task_1a.py
* Functions:		readImage, solveMaze
* 					[ Comma separated list of functions in this file ]
* Global variables:	CELL_SIZE
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
struct sockaddr_in dest_addr;
struct sockaddr_in source_addr;

char rx_buffer[RX_BUFFER_SIZE] = {0}; // buffer to store data from client
char tx_buffer[RX_BUFFER_SIZE];				// buffer to store data to be sent to client

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

	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;
	bzero(&dest_addr.sin_zero, 0);

	int my_sock, cli, len = sizeof(struct sockaddr_in);

	if ((my_sock = socket(addr_family, SOCK_STREAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1);
	}

	if ((bind(my_sock, (struct sockaddr *)&dest_addr, len)) == -1)
	{
		perror("bind: ");
		exit(-1);
	}

	if ((listen(my_sock, 5)) == -1)
	{
		perror("listen: ");
		exit(-1);
	}

	printf("Server started and listening to accept connections:\n");
	if ((cli = accept(my_sock, (struct sockaddr *)&source_addr, &len)) == -1)
	{
		perror("accept: ");
		exit(-1);
	}

	printf("Accepted connection from: %s\n", inet_ntoa(source_addr.sin_addr));

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

	int sent_bytes, recv_bytes, read_upto = 0, recd_file_num, no_obs = 0;
	int current_file_num = 0;

	printf("Going in recv lo\n");
	memset(rx_buffer, 0, RX_BUFFER_SIZE);

	while (1)
	{
		recv_bytes = recv(cli, rx_buffer, sizeof(rx_buffer) - 1, 0);
		if (recv_bytes > 0)
		{
			printf("Recvd %d bytes\n", recv_bytes);
			printf("Recd Data: %s\n", rx_buffer);
			recd_file_num = (int)rx_buffer[0] - 48;

			fseek(input_fp, 0, SEEK_SET);
			while (fgets(line_data, MAXCHAR, input_fp) != NULL)
			{
				current_file_num = (int)line_data[0] - 48;
				if (current_file_num != recd_file_num)
				{
					no_obs = 1;
				}
				else
				{
					no_obs = 0;
					break;
				}
			}

			if (read_upto == strlen(line_data) - 1 || no_obs == 1)
			{
				char end[5] = "@$@";
				sent_bytes = send(cli, end, strlen(end), 0);
				printf("sent %d bytes to client: %s\n", sent_bytes, inet_ntoa(source_addr.sin_addr));
				printf("End Request sent: %s\n", end);
				break;
			}
			fputs(rx_buffer, output_fp);
			fputs("\n", output_fp);
			memset(rx_buffer, 0, RX_BUFFER_SIZE);
			//obs
			char obs[10], obs_i = 0;
			for (int i = read_upto; i <= strlen(line_data); i++)
			{
				if (line_data[i] == '(')
				{
					obs[obs_i++] = '@';
					for (int j = i;; j++)
					{
						if (line_data[j] == ';' || line_data[j] == '\n' || line_data[j] == '\0')
						{
							if (line_data[j] == '\0')
							{
								read_upto = j - 1;
							}
							else
							{
								read_upto = j;
							}

							break;
						}
						obs[obs_i++] = line_data[j];
						// obs_i++;
						read_upto = j;
					}
					obs[obs_i] = '@';
					break;
				}
			}

			sent_bytes = send(cli, obs, strlen(obs), 0);
			printf("sent %d bytes to client: %s\n", sent_bytes, inet_ntoa(source_addr.sin_addr));
			printf("Obstacle sent: %s\n", obs);
		}
	}

	if (recd_file_num == 4)
	{
		fputs(rx_buffer, output_fp);
		fputs("\n", output_fp);
		fclose(input_fp);
		fclose(output_fp);
		exit(0);
	}
	return 0;
}

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
	// fgets(line_data, MAXCHAR, input_fp);
	// fgets(line_data, MAXCHAR, input_fp);
	// printf("%c", line_data[4]);
	// char *data = give_obs_coord();
	// for (int i = 0; i < strlen(data); i++)
	// {
	// 	printf("%c\n", data[i]);
	// }

	// printf("%ld\n", strlen(line_data));
	// printf("%d", line_data[15]);

	// int i = 0;
	// fseek(input_fp, 0, SEEK_SET);
	// while (fgets(line_data, MAXCHAR, input_fp) != NULL)
	// {
	// 	printf("%s", line_data);
	// }
	// if (line_data[10] == '\0')
	// {
	// 	printf("\n%ld\n", strlen(line_data));
	// }

	// printf("%ld\n", strlen(line_data));
	//	@(4,7)@
	//	@$@
	// fgets(line_data, MAXCHAR, input_fp);
	// printf("%s", line_data);
	// fgets(line_data, MAXCHAR, input_fp);
	// printf("%s", line_data);
	while (1)
	{

		// 	// Receive and send data from client and get the new shortest path
		// 	//kya ye function ek hi baar chlega?
		printf("\ncalling the func\n");
		receive_from_send_to_client(sock);
		printf("Writing to file\n");

		// 	// NOTE: YOU ARE NOT ALLOWED TO MAKE ANY CHANGE HERE
		// 	//buffer mai sab store krva kr hum sb ek baar mai pel denge
		fputs(rx_buffer, output_fp);
		fputs("\n", output_fp);
	}

	return 0;
}
