#include <stdlib.h>
#include <stdio.h>

#include <minicoredumper.h>

struct interesting_struct {
	int a, b, c;
	int d[3];
	const char *name;

	mcd_dump_data_t *save;
};

#define DUMP_ON_CRASH (1 << 0)

static int
register_interesting_struct(struct interesting_struct *s)
{
	mcd_dump_data_t *save = (mcd_dump_data_t *) malloc(sizeof (mcd_dump_data_t));

	if (save) {
		int ret;

		ret = mcd_dump_data_register_bin(NULL, /* ident */
						 0, /* dump_scope */
						 save, /* saveptr */
						 s, /* data_ptr */
						 (enum mcd_dump_data_flags) (MCD_DATA_PTR_DIRECT | MCD_LENGTH_DIRECT), /* ptr_flags */
						 sizeof (struct interesting_struct)); /* data_size */

		if (ret == 0) {
			s->save = save;
		} else {
			free(save);
			fprintf(stderr,
				"Error registering structure for minicoredumping.\n");
			return 0;
		}
	} else {
		fprintf(stderr,
			"Couldn't allocate a mcd_dump_data_t structure.\n");
		return 0;
	}

	return 1;
}

static struct interesting_struct *
new_interesting_struct(int value, const char *name, int flags) {
	struct interesting_struct *s;

	s = (struct interesting_struct *) malloc(sizeof(struct interesting_struct));

	if (s == NULL) {
		return NULL;
	}

	s->a = value * 1;
	s->b = value * 2;
	s->c = value * 3;

	s->d[0] = value * 4;
	s->d[1] = value * 5;
	s->d[2] = value * 6;

	s->name = name;

	if (flags & DUMP_ON_CRASH) {
		register_interesting_struct(s);
	}

	return s;
}

int main() {
	struct interesting_struct *x, *y, *z;

	x = new_interesting_struct(10, "xxxxxxxxxx", DUMP_ON_CRASH);
	y = new_interesting_struct(20, "yyyyyyyyyy", 0);
	z = NULL;

	x->a++;
	y->a++;
	z->a++;

	return 0;
}
