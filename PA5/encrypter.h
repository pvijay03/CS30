// versions 1.0 05/22/2022
#ifndef _cipher_h
#define _cipher_h 1
#define ENCRYPT_MODE	1
#define DECRYPT_MODE	0
#define	EXIT_OK		0
#define	EXIT_FAIL	-1
#ifndef __ASSEMBLER__
int setup(int, char **, int *, FILE **, FILE **, FILE **);
int rdbuf(char *, char *, FILE *, FILE *, int);
int encrypt(char *, char *, int);
int decrypt(char *, char *, int);
int wrbuf(char *, int, FILE *);
int cleanup(int);
void errmsg(char *);
#else
#define EXIT_FAILURE	1
#define EXIT_SUCCESS	0
#endif
#endif
