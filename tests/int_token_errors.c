#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main() {
	const char *src = "0x 0 0b123 0b 099 \n 9 0bb";

	struct lexer_t lexer;
	struct token_t token;
	lexer_init(&lexer, "<stdin>", src);

	// get all the tokens from the lexer
	int err = 0, lexer_errors = 0;
	while (1) {
		err = lexer_next(&lexer, &token);
		if (err == LEXER_EOF_ERR) break;
		if (err == LEXER_UNEXPECTED_ERR) {
			lexer_errors++;
			lexer_print_err(&lexer);
			printf("\n");
		}
	}

	// check if there were any errors from lexer
	if (lexer_errors) {
		printf("Total errors: %d\n", lexer_errors);
		exit(1);
	}

	lexer_free(&lexer);

	return 0;
}
