#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>

#include "http.h" 
#include "gif.h"

bool     env_cptc_comp;
char*    env_token;
char*    env_ip;
uint16_t env_port; 

static void init_env(int argc, char* argv[]) {
	env_cptc_comp = getenv("GPET_CPTC_COMPART") == NULL ? false : true;	
	
#	define GET_ENV(name, default) getenv(name) == NULL ? default : getenv(name)

	if (env_cptc_comp) {
		env_token = GET_ENV("CPTC_TOKEN", "alo insert token blyat");
	} else {
		env_token = GET_ENV("GPET_DISCORD_TOKEN", "insert token please");
	}

	char* str_port = GET_ENV("GPET_PORT", "8080");
	if (sscanf(str_port, "%hu", &env_port) != 1) env_port = 8080; 
}

extern bool http_runned;

// FIXME: fix error message in debug pages
static void process_system_urls(const char* url, int client_fd) {
	char* message = (char*) malloc(BUFFER_SIZE * sizeof(char));
	memset(message, '\0', BUFFER_SIZE * sizeof(char));

	if (strcmp(url, "server_info") == 0) {
		sprintf(message, "Port: %u\n", env_port); 
	} else if (strcmp(url, "server_exit") == 0) {
		sprintf(message, "Exiting\n"); 
		http_runned = false;
	} else {
		sprintf(message, "System page not found\n");
	}

	http_send_data(client_fd, "text/html", (uint8_t*) message, strlen(message));
	free(message);
}

static void process_gifs(const char* name, int client_fd) {
	uint8_t* gif;
	size_t gif_size;
	gif_gen(NULL, &gif, &gif_size);

	http_send_data(client_fd, "image/gif", gif, gif_size); 

	free(gif);
}

static void hendler(char* buf, int client_fd) {
	if (buf == NULL) goto fail;

	printf("%s\n", buf);

	char* metod = strtok(buf, "/");
	char* filename = strtok(NULL, "/");

	if (metod == NULL || filename == NULL) goto fail;

	const char* name   = strtok(filename, "."); 
	const char* format = strtok(NULL, ".");	

	if (name == NULL || format == NULL) goto fail;

	if (strcmp(format, "gif") == 0) {
		process_gifs(name, client_fd);
	} else if (strcmp(format, "sys") == 0) {
		process_system_urls(name, client_fd);
	} else {
		goto fail;
	}

fail:
	http_send_error(client_fd);
}

int main(int argc, char* argv[]) {
	init_env(argc, argv);
	gif_init();

	http_start_server("127.0.0.1", env_port, hendler);
}
