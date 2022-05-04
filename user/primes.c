#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int *p) {
	int num;
	close(p[1]);
	if (read(p[0], &num, sizeof(num)) == 0) {
		close(p[0]);
		exit(0);
	}
	
	int p_next[2];
	pipe(p_next);
	int pid = fork();
	if (pid == 0) {
		sieve(p_next);
	} else if (pid > 0) {
		int prime = num;
		printf("prime %d\n", num);
		close(p_next[0]);
		while (read(p[0], &num, sizeof(num))) {
			if (num % prime != 0) {
				write(p_next[1], &num, sizeof(num));
			}
		}
		close(p_next[1]);
		close(p[0]);
		wait((int *) 0);
		exit(0);
	} else {
		fprintf(2, "fork error");
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	int p[2];
	pipe(p);
	
	int pid = fork();
	if (pid == 0) {
		sieve(p);
	} else if (pid > 0) {
		close(p[0]);
		for (int i = 2; i <= 35; i++) {
			write(p[1], &i, sizeof(i));
		}
		close(p[1]);
		wait((int *) 0);
	} else {
		fprintf(2, "fork error");
		exit(1);
	}
	exit(0);
}

// By this assignment, I realize that children process will copy the
// memory and executes the code below fork()