// vim: noet ts=4 sw=4
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <38-moths/38-moths.h>

int main_sock_fd;
vector *printer_facts = NULL;

static int index_handler(const http_request *request, http_response *response) {
	if (printer_facts == NULL)
		return 500;

	const long int random_int = random() % printer_facts->count;
	char *random_fact = *(char **)vector_get(printer_facts, random_int);

	char full_fact[1024] = {0};
	snprintf(full_fact, sizeof(full_fact), "{\"facts\": [\"%s\"]}", random_fact);

	response->out = (unsigned char *)strndup(full_fact, sizeof(full_fact));
	response->outsize = strnlen(full_fact, sizeof(full_fact));

	return 200;
}

static const route all_routes[] = {
	{"GET", "root_handler", "^/.*$", 0, &index_handler, &heap_cleanup},
};

static int init_facts() {
	printer_facts = vector_new(sizeof(char *), 267);
	if (printer_facts == NULL)
		return -1;

	FILE *facts = NULL;
	facts = fopen("./all_facts_simple.txt", "r");
	if (facts == NULL)
		return -1;

	char line[512] = {0};
	while (fgets(line, sizeof(line), facts)) {
		char *duped = strndup(line, sizeof(line));
		duped[strnlen(duped, sizeof(line)) - 1] = '\0';
		vector_append_ptr(printer_facts, duped);
	}

	fclose(facts);
	return 0;
}

int main(int argc, char *argv[]) {
	srandom(time(NULL));

	if (init_facts() != 0)
		return -1;

	http_serve(&main_sock_fd, 8666, 2, all_routes, sizeof(all_routes)/sizeof(all_routes[0]));

	return 0;
}
