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
char current_file_num = '0';

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

	int my_sock;

	if ((my_sock = socket(addr_family, SOCK_STREAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1);
	}

	if ((bind(my_sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in))) == -1)
	{
		perror("bind: ");
		exit(-1);
	}

	if ((listen(my_sock, 5)) == -1)
	{
		perror("listen: ");
		exit(-1);
	}

	printf("Server started:\n");

	return my_sock;
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
int receive_from_send_to_client(int sock)
{
	int cli, len = sizeof(struct sockaddr_in), sent_bytes, recv_bytes;
	printf("Getting ready to accept:\n");
	if ((cli = accept(sock, (struct sockaddr *)&source_addr, &len)) == -1)
	{
		perror("accept: ");
		exit(-1);
	}

	//while loop for receving
	//anant tk recv krega

	//obs[] = ["@(4,7)@","@(1,8)","@$@"]
	//i=0
	char rx[1024] = {0};
	int read_upto = 0;
	printf("Going in recv lo\n");
	while (1)
	{
		recv_bytes = recv(cli, rx, sizeof(rx) - 1, 0);
		printf("Recvd %d bytes\n", recv_bytes);
		if (recv_bytes > 0)
		{
			printf("Recd Data: %c", rx[0]);
			printf("bach");
			//obs
			char obs[10], obs_i = 0;
			for (int i = read_upto; i < strlen(line_data); i++)
			{
				if (line_data[i] == '(')
				{
					obs[obs_i++] = '@';
					for (int j = i; line_data[j] != ';' || line_data[j] != '\n'; j++)
					{
						obs[obs_i++] = line_data[j];
						// obs_i++;
						read_upto = j;
					}
					obs[obs_i] = '@';
					break;
				}
			}
			printf("Obstacle to send: %s\n", obs);
		}
		printf("loop mai ni gye\n");
	}

	// sent_bytes = send(cli, rx_buffer, strlen(rx_buffer), 0);
	// printf("sent %d bytes to client: %s\n", sent_bytes, inet_ntoa(source_addr.sin_addr));
	return 0;
}

// char *give_obs_coord()
// {
// 	static char x[400];
// 	// if (current_file_num == line_data[0])
// 	// {
// 	//extracting all obstacles from the line
// 	int j = 0;
// 	for (int i = 0; i < strlen(line_data); i++)
// 	{
// 		if (line_data[i] == '(')
// 		{
// 			x[j] = '@';
// 			j = j + 1;
// 			// 	{int n = 0,l=1;
// 			for (int k = i; line_data[k] != ')'; k++)
// 			{
// 				x[j] = line_data[k];
// 				j = j + 1;

// 				if (line_data[k + 1] == ')')
// 				{
// 					x[j] = line_data[k + 1];
// 					x[j + 1] = '@';
// 					j = j + 2;
// 				}
// 			}
// 		}
// 		// 		{
// 		// 			char num = ' '; //num =''
// 		// 			if (line_data[k] == ',')
// 		// 			{
// 		// 				x[j] = num;
// 		// 				j++;
// 		// 			}
// 		// 			num = num + line_data[k];
// 		// 			// x[j] = x[j]*l +line_data[k]*n
// 		// 			// l = l*10
// 		//			//	n+=1
// 		// 			// if((int)line_data[k] >=48 && (int)line_data[k] <=57)
// 		// 		}
// 		//			l =10; n = 0;
// 		// 		// function call shortest path
// 		// 		// x[j] reset
// 		// 	}
// 		// }
// 		// x[j] = "14";
// 		// x = [4,7,1,8,$]
// 		// }
// 		return x;
// 	}
// }

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
	// while (i < 4)
	// {
	// 	printf("%s", line_data);
	// 	fgets(line_data, MAXCHAR, input_fp);
	// 	++i;
	// }
	// printf("%ld\n", strlen(line_data));
	//	@(4,7)@
	//	@$@
	// fgets(line_data, MAXCHAR, input_fp);
	// printf("%s", line_data);
	// fgets(line_data, MAXCHAR, input_fp);
	// printf("%s", line_data);
	// while (1)
	// {

	// 	// Receive and send data from client and get the new shortest path
	// 	//kya ye function ek hi baar chlega?
	printf("calling the func");
	receive_from_send_to_client(sock);

	// 	// NOTE: YOU ARE NOT ALLOWED TO MAKE ANY CHANGE HERE
	// 	//buffer mai sab store krva kr hum sb ek baar mai pel denge
	// 	fputs(rx_buffer, output_fp);
	// 	fputs("\n", output_fp);
	// }

	return 0;
}
