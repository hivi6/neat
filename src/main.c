#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

void usage() {
	fprintf(stderr, 
		"Error: Expected file\n"
		"\n"
		"Usage: \n"
		"\tneat <filename>\n"
	);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		usage();
		exit(1);
	}

	// open the file
	const char *filename = argv[1];
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: Couldn't open file: '%s'.\n", filename);
		exit(1);
	}

	// get the size of the file
	fseek(fp, 0, SEEK_END);
	int src_sz = ftell(fp);

	// reset the file pointer of the file
	fseek(fp, 0, SEEK_SET);

	// allocate memory for file content
	char *src = (char*) malloc(src_sz * sizeof(char));
	if (src == NULL) {
		fprintf(stderr, "Error: Something went wrong with malloc.\n");
		exit(1);
	}

	// read the content of the file
	int i = 0, ch;
	while ((ch = fgetc(fp)) != EOF) {
		src[i++] = ch;
	}
	src[i] = 0;

	// close the file
	fclose(fp);

	// initialise the lexer
	struct lexer_t lexer;
	struct token_t token;
	lexer_init(&lexer, "<stdin>", src);

	// initialise the parser
	struct parser_t parser;
	parser_init(&parser);

	// get all the tokens from the lexer
	int err = 0, lexer_errors = 0;
	while (1) {
		err = lexer_next(&lexer, &token);
		if (err == LEXER_EOF_ERR) break;
		if (err == LEXER_UNEXPECTED_ERR) {
			lexer_errors++;
			lexer_print_err(&lexer);
			fprintf(stderr, "\n");
		}
		else {
			parser_append(&parser, token);
		}
	}

	// check if there were any errors from lexer
	if (lexer_errors) {
		fprintf(stderr, "Total errors: %d\n", lexer_errors);
		exit(1);
	}

	// now that all the tokens are loaded, parse the tokens
	struct ast_t *ast;
	parser_parse(&parser, &ast);
	if (ast == NULL) {
		// something went wrong while parsing, print the errors
		// TODO: print the parser errors
		fprintf(stderr, "Something went wrong while parsing\n");
		exit(1);
	}

	// print the ast
	ast_print(ast);

	// free stuffs
	ast_free(ast);
	parser_free(&parser);
	lexer_free(&lexer);

	// free the content of the file
	free(src);

	return 0;
}
