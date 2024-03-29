#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793

#define SHELL_LIMIT 256

#define STACK_LENGTH 64
double stack[STACK_LENGTH];
unsigned int stackpos;

enum {
	SUCCESS,
	ERR_EXIT, // Exit (intentional, not a true error)
	ERR_EXC_DEC, // Excessive Decimals
};

inline void push(double value) {
	stack[stackpos++] = value;
	return;
}

inline double pop(void) {
	if (stackpos <= 0) return NAN;
	return stack[--stackpos];
}

void swap(void) {
	double a = pop();
	double b = pop();
	push(a);
	push(b);
}

double peek(void) {
	if (stackpos <= 0) return NAN;
	return stack[stackpos-1];
}

#define token_LIMIT 32
char token[token_LIMIT];
unsigned int tokenpos = 0;

#define WORD_LIMIT 60
typedef struct Word {
	char* name;
	char* value;
}Word;

int interpret(char* src) {

	unsigned int i = 0;
	char c;

	int in_num = 0;
	int in_dec = 0;
	int dec_place = 0;
	int in_word_n = 0;
	int in_word_v = 0;
	for (;;) {
		c = src[i];

		if (c == '\0' || c == ' ' || c == '\n') {
			in_num = 0;
			in_dec = 0;
			dec_place = 0;
			if (tokenpos) {
				if (!strcmp(token, "+")) {
					push(pop() + pop());
				}

				else if (!strcmp(token, "-")) {
					swap();
					push(pop() - pop());
				}

				else if (!strcmp(token, "*")) {
					push(pop() * pop());
				}

				else if (!strcmp(token, "/")) {
					swap();
					push(pop() / pop());
				}

				else if (!strcmp(token, ".")) {
					printf("%.15g", pop());
				}

				else if (!strcmp(token, ":")) {
					in_word_n = 1;
				}

				else if (!strcmp(token, "sin")) {
					push(sin(pop()));
				}
					
				else if (!strcmp(token, "cos")) {
					push(cos(pop()));
				}

				else if (!strcmp(token, "tan")) {
					push(tan(pop()));
				}

				else if (!strcmp(token, "asin")) {
					push(asin(pop()));
				}

				else if (!strcmp(token, "acos")) {
					push(acos(pop()));
				}

				else if (!strcmp(token, "atan")) {
					push(atan(pop()));
				}

				else if (!strcmp(token, "PI")) {
					push(PI);
				}

				else if (!strcmp(token, "torad")) {
					push(pop() * PI / 180);
				}

				else if (!strcmp(token, "todeg")) {
					push(pop() * 180 / PI);
				}

				else if (!strcmp(token, "!!!")) {
					return 1;
				}

				
			}

			for (int i = 0; i < token_LIMIT; i++) {
				token[i] = 0;
			}
			tokenpos = 0;

			if (c == '\0') break;
		}

		// A number is continuing
		else if (in_num) {
			if (c >= '0' && c <= '9') {
				int current = c - '0';

				if (in_dec) {
					push(pop() + (double)current / (double)pow(10, dec_place));
					dec_place++;
				}
				else {
					push(pop() * 10 + current);
				}
			}

			else if (c == '.') {
				if (in_dec) {
					printf("Error: Too many decimal signs\n");
					return ERR_EXC_DEC;
				}
				in_dec = 1;
				dec_place = 1;
			}
		}

		// A number is starting
		else if (c >= '0' && c <= '9') {
			push(c - '0');
			in_num = 1;
		}

		else {
			token[tokenpos++] = c;
		}


		i++;
	}

	return SUCCESS;
}

int main(int argc, char** argv) {
	// Shell mode
	if (argc == 1) {
		char line[SHELL_LIMIT];
		int result = 0;

		printf("Welcome to the FORTH shell!\n");
		printf("Type '!!!' anywhere to exit the shell. (or ctrl+c)\n");
		do {
			printf("-> ");
			fgets(line, SHELL_LIMIT, stdin);
			result = interpret(line);
			putchar('\n');
		} while (result == 0);
		if (result > ERR_EXIT)
			printf("Error found! Code: %i\n", result);
		return 0;
	}
	return 0;
}
