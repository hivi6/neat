#include "token.h"
#include "ast.h"

struct token_t get_token(int type, const char *src) {
	struct pos_t start = (struct pos_t){.idx=0, .ln=1, .col=1};
	struct pos_t end = start;
	while (src[end.idx]) pos_next(&end, src[end.idx]);

	return (struct token_t) {
		.type=type,
		.src=src,
		.filename="<stdin>",
		.start=start,
		.end=end,
	};
}

int main() {
	struct ast_t *ast = ast_literal(get_token(TT_DECIMAL, "123"));

	ast_print(ast);
	ast_free(ast);

	return 0;
}
