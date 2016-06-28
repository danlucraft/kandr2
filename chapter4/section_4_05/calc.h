#define NUMBER  '0' /* signal that a number was found */
#define OP      '1' /* signal that it's an operation / command */

int getop(char []);
void push(double);
double pop(void);
void clear(void);
int getch(void);
void ungetch(int);
int streq(char* a, char* b);

