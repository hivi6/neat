#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

int main() {
	const char *src = "(0x1f+0)*0123\n+9       - (0b0110 - 2 * 3) +";

	struct lexer_t lexer;
	struct token_t token;
	lexer_init(&lexer, "<stdin>", src);

	struct parser_t parser;
	parser_init(&parser);

	int err;
	while (!(err = lexer_next(&lexer, &token))) {
		parser_append(&parser, token);
	}

	if (err == LEXER_UNEXPECTED_ERR) {
		lexer_print_err(&lexer);
		exit(1);
	}

	printf("capacity of tokens: %d | number of tokens: %d\n", parser.cap, 
		parser.sz);
	for (int i = 0; i < parser.sz; i++) {
		printf("address[%d] = %p | token = ", i, &parser.tokens[i]);
		token_print(&parser.tokens[i]);
		printf("\n");
	}

	struct ast_t *ast;
	parser_parse(&parser, &ast);
	ast_print(ast);
	ast_free(ast);

	parser_free(&parser);
	lexer_free(&lexer);

	return 0;
}
