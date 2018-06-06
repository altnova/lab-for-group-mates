#include <stdio.h>

int main()
{
	FILE *f = fopen("file.txt", "w+b");
	fseek(f, 0, SEEK_SET);
	return 0;
}