#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(2, "xargs: mininum amount of arguments is 2\n");
		exit(1);
	}
	
	if (argc- 1 > MAXARG) {
		fprintf(2, "maximum amount of arguments is %d\n", MAXARG);
		exit(1);
	}
	
	char *cargv[MAXARG];
	
	for (int i = 1; i < argc; i++) {
		cargv[i - 1] = argv[i];
	}
	
	char buf[512];
	int i = 0;
	int index = argc - 1; // index of cargv
	int end = 0;
	
	while (read(0, &buf[i], sizeof(char))) {
		if (buf[i] == ' ' || buf[i] == '\n') {
			if (buf[i] == '\n')
				end = 1;
			buf[i] = 0; // end signal
			cargv[index++] = buf;
			i = 0;
		} else {
			i++;
		}
		// exec for every lines
		if (end) {
			end = 0;
			cargv[index] = 0; // end signal
			index = argc - 1; // reindex for next line
			int pid = fork();
			if (pid == 0) {
				exec(cargv[0], cargv);
			} else if (pid > 0) {
				wait((int *) 0);
			} else {
				fprintf(2, "fork error\n");
				exit(1);
			}
		}
	}
	exit(0);
}