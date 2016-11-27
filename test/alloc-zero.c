#include <stdlib.h>

int main(void)
{
	void		*a;
	void		*b;
	void		*c;

	/* simple allocation */
	a = malloc(0);
	free(a);

	/* will be freed at the end */
	c = malloc(0);

	/* malloc(0) + realloc(0) */
	a = malloc(0);
	b = realloc(a, 0);
	free(b);

	/* malloc(0) + realloc(1) */
	a = malloc(0);
	b = realloc(a, 1);
	free(b);

	/* malloc(1) + realloc(0) */
	a = malloc(1);
	b = realloc(a, 0);
	free(b);

	free(c);
}
