#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int num = 789;
	printf("123\n");
	int pid = fork();
	if (pid == 0) {
		printf("%d", num);
		printf("child: %d\n", getpid());
	} else if (pid > 0) {
		wait((int *) 0);
		printf("parent: %d\n", getpid());
	}
	printf("456\n");
	exit(0);
}