#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(2, "Usage: sleep seconds\n");
		exit(1);
	}
	
	int sleep_sec = atoi(argv[1]);
	if (sleep_sec > 0) {
		sleep(sleep_sec);
	} else {
		fprintf(2, "Invalid interval %s\n", argv[1]);
	}
	
	exit(0);
}