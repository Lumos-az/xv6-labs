#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
	static char buf[DIRSIZ+1];
	char *p;
	
	// Find first character after last slash.
	for(p = path + strlen(path); p >= path && *p != '/'; p--)
	;
	p++;
	
	memmove(buf, p, strlen(p) + 1);
	return buf;
}

void find(char *path, char *targetName) {
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	if ((fd = open(path, 0)) < 0) {
		fprintf(2, "Can\'t open %s\n", path);
	}
	
	if (fstat(fd, &st) < 0) {
		fprintf(2, "Can\'t stat %s\n", path);
		close(fd);
		return;
	}
	
	switch (st.type) {
		case T_FILE:
			if (strcmp(fmtname(path), targetName) == 0)
				printf("%s\n", path);
			break;
		
		case T_DIR:
			if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
				fprintf(2, "Path too long\n");
				break;
			}
			
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			
			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (de.inum == 0)
					continue;
				if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
					continue;
				
				memmove(p, de.name, DIRSIZ);
				p[strlen(de.name)] = 0;
				
				if (stat(buf, &st) < 0) {
					fprintf(2, "Can\'t stat %s", buf);
				}
				
				find(buf, targetName);
				
			}
			break;
	}
	
	close(fd);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(2, "Wrong Usage!\n");
		exit(1);
	}
	
	find(argv[1], argv[2]);
	exit(0);
}