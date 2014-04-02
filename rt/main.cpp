#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	printf("hi\n");
	system("root -l graph.C &> /dev/null");
	system("echo asdf");
	return 0;
}
