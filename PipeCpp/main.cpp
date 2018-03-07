#include <stdio.h>
#include "PipeCpp.h"

int main() {
	int choose=0;
	if (choose) {
		PipeServer pipe;
		while (true) {
			pipe.CreatePipe();
		}
	}
	else {
		char buf[PIPE_BUF_SIZE] = { 0, };
		strcpy_s(buf, "AA");
		PipeClient pipe;
		while (buf[0] < 'z'+1) {
			if(!pipe.ConnectPipe())
				continue;
			printf("<<%s\n", buf);
			if (pipe.WritePipe(buf, strlen(buf)))
				break;
			if (pipe.ReadPipe(buf))
				break;
			printf(">>%s\n", buf);
			buf[0] = buf[0] + 1;
		}
	}
	return 0;
}