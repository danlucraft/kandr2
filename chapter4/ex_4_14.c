/*

Define a macro s w a p ( t , x , y ) that interchanges two arguments of type 
t. (Block structure will help.)

*/

#include <stdio.h>
#define swap(t, x, y) {\
	t tmp = y;\
	y = x;\
	x = tmp;\
}

int main()
{
	int x = 5;
	int y = 100;

	swap(int, x, y);
	printf("x = %d, y = %d\n", x, y);
}
