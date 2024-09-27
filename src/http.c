#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <regex.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "http.h"

#define error(msg, code) do { \
	perror(msg);              \
	exit(code);               \
} while(1)

typedef struct {
	int client_fd;
	http_hendler_t hendler;
} http_rec_args_t;

static void* http_rec(void* args);

void http_start_server(const char* ip, const uint16_t port, http_hendler_t hendler) {
	int                server_fd;
	struct sockaddr_in address;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) error("socket failed", EXIT_FAILURE);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) error("bind failed", EXIT_FAILURE);
	if (listen(server_fd, 3) < 0)                                                  error("listen failed", EXIT_FAILURE);

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t          client_addr_len = sizeof(client_addr);
		http_rec_args_t*   http_rec_args   = malloc(sizeof(http_rec_args_t));
		
		if ((http_rec_args->client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len)) < 0) continue; 
		http_rec_args->hendler        = hendler;

		pthread_t thread_id;
		pthread_create(&thread_id, NULL, http_rec, (void*) http_rec_args);	
		pthread_detach(thread_id);
	}
}

#define BUFFER_SIZE 1024

static void* http_rec(void* args) {
	http_rec_args_t* args_struct = (http_rec_args_t*) args;

	char* buf = (char*) malloc(BUFFER_SIZE * sizeof(char));
	ssize_t rec_size = recv(args_struct->client_fd, buf, BUFFER_SIZE, 0);

	if (rec_size <= 0) goto done;

	regex_t regex;
	regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);	
	regmatch_t matches[2];

	if (regexec(&regex, buf, 2, matches, 0) == 0) {
		buf[matches[1].rm_eo] = '\0';
		args_struct->hendler(buf);
	}

	regfree(&regex);

done:
	close(args_struct->client_fd);
	free(args);
	free(buf);

	return NULL;
}	
