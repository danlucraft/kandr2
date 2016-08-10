/*



*/

#include <sys/file.h> // contains open and O_RDONLY etc
#include <unistd.h>   // contains read, write and close

#include <stdlib.h> // for malloc
#include <string.h> // for strlen

#include <errno.h>

//#define NULL      0 // already defined
#define EOF       (-1)
#define BUFSIZ    1024
#define OPEN_MAX  20    // max files open at once

typedef struct _iobuf {
	ssize_t  cnt;	  // characters left
	char	 *ptr;	  // next character position
	char	 *base;	  // location of buffer
	int		 flag;	  // mode of file access
	int		 fd;	  // file descriptor
} FILE;

enum flags {
	_READ = 01,	  // file open for reading
	_WRITE = 02,  // file open for writing
	_UNBUF = 04,  // file is unbuffered
	_EOF = 010,	  // EOF has occurred on this file
	_ERR = 020	  // error occurred on this file
};

extern FILE _iob[OPEN_MAX];
FILE _iob[OPEN_MAX] = {
	{ 0, (char *) 0, (char *) 0, _READ, 0 },
	{ 0, (char *) 0, (char *) 0, _WRITE, 1 },
	{ 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 }
};

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

#define feof(p)	  (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)

#define getc(p)	  (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x),p))

#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)

#define PERMS 0666 // rw for owner, group, others

void prints(char *s);
void prints(char *s)
{
	write(stdout->fd, s, strlen(s));
}

// fopen: open file, return file ptr
FILE *fopen(char *name, char *mode);
FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;

	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if ((fp->flag & (_READ | _WRITE)) == 0)
			break; // found free slot

	if (fp >= _iob + OPEN_MAX) // no free slots
		return NULL;

	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else {
		fd = open(name, O_RDONLY, 0);
	}

	if (fd == -1) // couldn't access name
		return NULL;

	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	fp->flag = (*mode == 'r') ? _READ : _WRITE;

	return fp;
}

int _fillbuf(FILE *fp);
int _fillbuf(FILE *fp)
{
	unsigned long bufsize;

	if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
		return EOF;

	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if (fp->base == NULL) // no buffer yet
		if ((fp->base = malloc(bufsize)) == NULL)
			return EOF; // can't allocate buffer

	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag |= _EOF;
		else
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

int fflush(FILE *fp);
int fflush(FILE *fp)
{
	ssize_t bufsize;

	if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
		return EOF;

	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

	if (fp->cnt != (ssize_t) bufsize)
		if (write(fp->fd, fp->base, (size_t) bufsize - (size_t) fp->cnt) == -1) {
			fp->flag |= _ERR;
			fp->cnt = 0;
			return EOF;
		}
	fp->ptr = fp->base;
	fp->cnt = (ssize_t) bufsize;

	return 1;
}

int _flushbuf(int c, FILE *fp);
int _flushbuf(int c, FILE *fp)
{
	ssize_t bufsize;

	if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
		return EOF;

	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

	if (fp->base == NULL) {// no buffer yet
		if ((fp->base = malloc((unsigned long) bufsize)) == NULL)
			return EOF; // can't allocate buffer
		fp->ptr = fp->base;
		fp->cnt = (ssize_t) bufsize;
	}
	fflush(fp);

	*(fp->ptr++) = (char) c;
	fp->cnt--;
	return c;
}

int fclose(FILE *fp);
int fclose(FILE *fp)
{
	int result = close(fp->fd);
	free(fp->base);
	fp->base = NULL;
	fp->flag = 0;
	return result;
}

int main()
{
	FILE *fin;
	FILE *fout, *fout2;

	if ((fin = fopen("chapter8/ex_8_03.c", "r")) == NULL)
		prints("error opening ex_8_03.c\n");
	prints("opened sec_8_05.c\n");

	if ((fout = fopen("test1", "w")) == NULL)
		prints("error opening test1 to write\n");

	int i = 0;
	int c;
	while ((c = getc(fin)) != EOF && i++ < 100) {
		putc((char) c, stdout);
		putc((char) c, fout);
	}
	putc('\n', stdout);
	putc('\n', fout);
	fflush(stdout);
	fflush(fout);
	fclose(fout);

	if ((fout2 = fopen("test2", "w")) == NULL)
		prints("error opening test2 to write\n");

	if (fout == fout2)
		prints("used same slot, fclose works!\n");
	else
		prints("used different slot\n");

	putc('a', fout2);
	putc('b', fout2);
	fflush(fout2);
}




