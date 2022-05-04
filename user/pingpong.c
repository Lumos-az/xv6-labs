#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
	int p2c[2];
	int c2p[2];
	pipe(p2c);
	pipe(c2p);
	char buf[1];
	
	int pid = fork();
	if (pid == 0) {
		close(p2c[1]);
		read(p2c[0], buf, sizeof(buf));
		printf("%d: received p%sng\n", getpid(), buf);
		close(p2c[0]);
		
		close(c2p[0]);
		write(c2p[1], "o", 1);
		close(c2p[1]);
	} else if (pid > 0) {
		close(p2c[0]);
		write(p2c[1], "i", 1);
		close(p2c[1]);
		
		close(c2p[1]);
		read(c2p[0], buf, sizeof(buf));
		printf("%d: received p%sng\n", getpid(), buf);
		close(c2p[0]);
	} else {
		printf("fork error");
	}
	
	exit(0);
}