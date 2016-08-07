/*

This just copies its input to its output

*/

#include <stdio.h>
#include <unistd.h>

int main()
{
	char buf[BUFSIZ];
	ssize_t n;

	while ((n = read(0, buf, BUFSIZ)) > 0)
		write(1, buf, (size_t) n);

	return 0;
}
