#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main() {
	const char *src = "123\n 4(56 ++--//** 78)9";

	struct lexer_t lexer;
	struct token_t token;
	lexer_init(&lexer, "<stdin>", src);

	int err;
	while (!(err = lexer_next(&lexer, &token))) {
		token_print(&token);
		printf("\n");
	}

	if (err == LEXER_UNEXPECTED_ERR) {
		lexer_print_err(&lexer);
		exit(1);
	}

	lexer_free(&lexer);

	return 0;
}
