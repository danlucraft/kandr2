/*

Rewrite the program cat using read, write, open, and close instead of their
standard library equivalents. Perform experiments to determine the relative
speeds of the two versions.

---

Runtimes

Old version using getc and putc.

$ clang -Weverything chapter7/sec_7_06_cat.c && time ./a.out chapter5/ex_5_07.test > /dev/null

real    0m0.096s
user    0m0.090s
sys     0m0.004s

New version with read and write:

$ clang -Weverything chapter8/ex_8_01.c && time ./a.out chapter5/ex_5_07.test > /dev/null

real    0m0.005s
user    0m0.001s
sys     0m0.003s

So 95% faster. I assume this is because of the use of a sizable buffer in this
one.

Indeed if you make BUFSIZ = 1 in this code, then the runtime is:

real    0m1.099s
user    0m0.184s
sys	    0m0.913s

So the relative runtimes are:

 * read and write with BUFSIZ chunks: 1
 * stdlib getc and putc:              20x slower
 * read and write with size 1 chunks: 220x slower

*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/file.h> // contains open and O_RDONLY etc
#include <unistd.h>   // contains read, write and close

void filecopy(int fin, int fout);
void filecopy(int fin, int fout)
{
	char buf[BUFSIZ];
	ssize_t n;
	while ((n = read(fin, buf, BUFSIZ)) > 0)
		write(fout, buf, (size_t) n);
}

int main(int argc, char *argv[])
{
	int fin;
	char *prog = argv[0]; // program name for errors
	
	if (argc == 1)
		filecopy(0, 1);
	else
		while (--argc > 0)

			if ((fin = open(*++argv, O_RDONLY, 0)) == -1) {
				fprintf(stderr, "%s: can't open %s\n", 
						prog, *argv);
				exit(1);
			} else {
				filecopy(fin, 1);
				close(fin);
			}
	if (ferror(stdout)) {
		fprintf(stderr, "%s: error writing stdout\n", prog);
		exit(2);
	}
	exit(0);
}

/*

$ clang -Weverything chapter8/ex_8_01.c && echo "foobar" | ./a.out chapter8/ex_8_01.c | tail
			} else {
				filecopy(fin, 1);
				close(fin);
			}
	if (ferror(stdout)) {
		fprintf(stderr, "%s: error writing stdout\n", prog);
		exit(2);
	}
	exit(0);
}

$ clang -Weverything chapter8/ex_8_01.c && echo "foobar" | ./a.out 
foobar

*/
