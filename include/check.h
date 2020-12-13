#ifndef HARE_CHECK_H
#define HARE_CHECK_H
#include <stdbool.h>
#include <stdint.h>
#include "identifier.h"
#include "types.h"

struct expression;

enum function_flags {
	FN_FINI = 1 << 0,
	FN_INIT = 1 << 1,
	FN_TEST = 1 << 2,
};

struct function_decl {
	const struct type *type;
	struct expression *body;
	char *symbol;
	uint32_t flags; // enum function_flags
};

enum declaration_type {
	DECL_FUNC,
	DECL_TYPE,
	DECL_GLOBAL,
	DECL_CONSTANT,
};

struct declaration {
	enum declaration_type type;
	struct identifier ident;
	bool exported;
};

struct declarations {
	struct declaration decl;
	struct declarations *next;
};

struct unit {
	struct identifier *ns;
	struct declarations *declarations;
};

struct ast_unit;

void check(const struct ast_unit *aunit, struct unit *unit);

#endif
