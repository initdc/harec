#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "identifier.h"
#include "util.h"

static int
_asprintf(char **strp, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int n = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	*strp = xcalloc(n + 1, 1);

	va_start(ap, fmt);
	n = vsnprintf(*strp, n + 1, fmt, ap);
	va_end(ap);
	return n;
}

char *
identifier_unparse(const struct identifier *ident)
{
	if (ident->ns) {
		char *ns = identifier_unparse(ident->ns);
		if (!ns) {
			return NULL;
		}
		char *str = NULL;
		_asprintf(&str, "%s::%s", ns, ident->name);
		free(ns);
		return str;
	}
	return strdup(ident->name);
}

int
identifier_unparse_static(const struct identifier *ident, char *buf, size_t len)
{
	assert(len < INT_MAX);
	if (ident->ns) {
		int prefix = identifier_unparse_static(ident->ns, buf, len);
		int n = snprintf(&buf[prefix], len - prefix,
				"::%s", ident->name);
		if (n >= (int)len) {
			buf[len - 1] = '\0';
		}
		return prefix + n;
	}
	int n = snprintf(buf, len, "%s", ident->name);
	if (n >= (int)len) {
		buf[len - 1] = '\0';
	}
	return n;
}

void
identifier_dup(struct identifier *new, const struct identifier *ident)
{
	assert(ident && new);
	new->name = strdup(ident->name);
	if (ident->ns) {
		new->ns = xcalloc(1, sizeof(struct identifier));
		identifier_dup(new->ns, ident->ns);
	}
}

bool
identifier_eq(const struct identifier *a, const struct identifier *b)
{
	if (!a && !b) {
		return true;
	} else if ((!a && b) || (a && !b)) {
		return false;
	}
	if (strcmp(a->name, b->name) != 0) {
		return false;
	}
	return identifier_eq(a->ns, b->ns);
}
