#include <stdlib.h>
#include <stdio.h>

#include <minicoredumper.h>

struct interesting_struct {
	int a, b, c;
	int d[3];
	const char *name;

	mcd_dump_data_t save;
};

#define DUMP_ON_CRASH (1 << 0)

static int
register_interesting_struct(struct interesting_struct *s)
{
	mcd_dump_data_t save;
	int ret;

	ret = mcd_dump_data_register_bin(NULL, /* ident */
					 0, /* dump_scope */
					 &save, /* saveptr */
					 s, /* data_ptr */
					 MCD_DATA_PTR_DIRECT | MCD_LENGTH_DIRECT, /* ptr_flags */
					 sizeof (struct interesting_struct)); /* data_size */

	if (ret != 0) {
		fprintf(stderr,
			"Error registering structure for minicoredumping.\n");
		return 0;
	}

	return 1;
}

static struct interesting_struct *
new_interesting_struct(int value, const char *name, int flags) {
	struct interesting_struct *s;

	s = malloc(sizeof(struct interesting_struct));

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

static char *array = NULL;

static void
test_stride()
{
	const size_t len = 1024;
	array = malloc(len);
	size_t i;
	char *p;
	mcd_dump_data_t save;
	int stride_len;

	mcd_dump_data_register_bin(NULL, 0, &save, &array,
			MCD_DATA_PTR_DIRECT | MCD_LENGTH_DIRECT, sizeof(array));

	for (i = 0; i < len; i++) {
		array[i] = (char) i;
	}

	p = array;

	for (stride_len = 1; stride_len <= 16; stride_len *= 2) {
		for (i = 0; i < 16; i++, p += stride_len) {
			if (i % 2)
				continue;


			mcd_dump_data_register_bin(NULL,
						   0,
						   &save,
						   p,
						   MCD_DATA_PTR_DIRECT
						   | MCD_LENGTH_DIRECT,
						   stride_len);

			if (i == 2)
				/* Test unregistering a data dump. */
				mcd_dump_data_unregister(save);
		}
	}
}

int main() {
	struct interesting_struct *x, *y, *z;

	test_stride();

	x = new_interesting_struct(10, "xxxxxxxxxx", DUMP_ON_CRASH);
	y = new_interesting_struct(20, "yyyyyyyyyy", 0);
	z = NULL;

	x->a++;
	y->a++;
	z->a++;

	return 0;
}
