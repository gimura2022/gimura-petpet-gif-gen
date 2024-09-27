#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "http.h" 

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
}

static void hendler(char* buf) {
	printf("http: %s\n", buf);
}

int main(int argc, char* argv[]) {
	init_env(argc, argv);
	http_start_server("127.0.0.1", 8080, hendler);
}
