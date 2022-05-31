/*
 *
 *  chirc: a simple multi-threaded IRC server
 *
 *  This module provides the main() function for the server,
 *  and parses the command-line arguments to the chirc executable.
 *
 */

/*
 *  Copyright (c) 2011-2020, The University of Chicago
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or withsend
 *  modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of The University of Chicago nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software withsend specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY send OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <errno.h>
#include "log.h"


int main(int argc, char *argv[])
{
    int opt;
    char *port = "6667", *passwd = NULL, *servername = NULL, *network_file = NULL;
    int verbosity = 0;

    while ((opt = getopt(argc, argv, "p:o:s:n:vqh")) != -1)
        switch (opt)
        {
        case 'p':
            port = strdup(optarg);
            break;
        case 'o':
            passwd = strdup(optarg);
            break;
        case 's':
            servername = strdup(optarg);
            break;
        case 'n':
            if (access(optarg, R_OK) == -1)
            {
                printf("ERROR: No such file: %s\n", optarg);
                exit(-1);
            }
            network_file = strdup(optarg);
            break;
        case 'v':
            verbosity++;
            break;
        case 'q':
            verbosity = -1;
            break;
        case 'h':
            printf("Usage: chirc -o OPER_PASSWD [-p PORT] [-s SERVERNAME] [-n NETWORK_FILE] [(-q|-v|-vv)]\n");
            exit(0);
            break;
        default:
            fprintf(stderr, "ERROR: Unknown option -%c\n", opt);
            exit(-1);
        }

    if (!passwd)
    {
        fprintf(stderr, "ERROR: You must specify an operator password\n");
        exit(-1);
    }

    if (network_file && !servername)
    {
        fprintf(stderr, "ERROR: If specifying a network file, you must also specify a server name.\n");
        exit(-1);
    }

    /* Set logging level based on verbosity */
    switch(verbosity)
    {
    case -1:
        chirc_setloglevel(QUIET);
        break;
    case 0:
        chirc_setloglevel(INFO);
        break;
    case 1:
        chirc_setloglevel(DEBUG);
        break;
    case 2:
        chirc_setloglevel(TRACE);
        break;
    default:
        chirc_setloglevel(TRACE);
        break;
    }

    /* Your code goes here */

    /* IMPORTANT: Like the oneshot-single.c, we are creating the socket and sockaddr
     * structures manually. Your solution _must_ use getaddrinfo instead. You can see
     * examples of this in client.c and in server-pthreads.c */

    int server_socket;
    int client_socket, nbytes;
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage *client_addr;
    //struct sockaddr_in server_addr, client_addr;
    int yes = 1;
    socklen_t sin_size = sizeof(struct sockaddr_in);

    char *msg = ":bar.example.com 001 user1 :Welcome to the Internet Relay Network user1!user1@foo.example.com\r\n";

/*
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, 5);
*/

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // Return my address, so I can bind() to it

    /* Note how we call getaddrinfo with the host parameter set to NULL */
    if (getaddrinfo(NULL, "23320", &hints, &res) != 0) {
        perror("getaddrinfo() failed");
        //pthread_exit(NULL);
		return 1;
    }

    for(p = res;p != NULL; p = p->ai_next)
    {
        if ((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("Could not open socket");
            continue;
        }

        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("Socket setsockopt() failed");
            close(server_socket);
            continue;
        }

		if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1) {
            perror("Socket bind() failed");
            close(server_socket);
            continue;
        }

        if (listen(server_socket, 5) == -1) {
            perror("Socket listen() failed");
            close(server_socket);
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (p == NULL)
    {
        fprintf(stderr, "Could not find a socket to bind to.\n");
        //pthread_exit(NULL);
		return 1;
    }

    while(1) {


        //client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &sin_size);
        //send(client_socket, msg, strlen(msg), 0);

        client_addr = calloc(1, sin_size);
        if ((client_socket = accept(server_socket, (struct sockaddr *) client_addr, &sin_size)) == -1) {
            /* If this particular connection fails, no need to kill the entire thread. */
            free(client_addr);
            perror("Could not accept() connection");
            continue;
        }

        nbytes = send(client_socket, msg, strlen(msg), 0);

        if (nbytes == -1 && (errno == ECONNRESET || errno == EPIPE))
        {
            fprintf(stderr, "Socket %d disconnected\n", client_socket);
            close(client_socket);
            return 1;
        }
        else if (nbytes == -1)
        {
            perror("Unexpected error in send()");
            return 1;
        }
        sleep(5);


    }

    close(server_socket);

    return 0;
}

