#ifndef test_H
#define test_H

struct test_list {
	char *name;
	int (*call)(void);
	struct test_list *next;
};

struct test_stat {
	int passed;
	int failed;
	size_t maxlen;
	struct test_list *test;
};

struct test_list *
test_list_add(struct test_list *tl, int (*call)(void), const char *name) {
	struct test_list *tmp = tl;
	tl = malloc(sizeof(*tl) + strlen(name) + 1);
	tl->name = (char *)(tl + 1);
	
	strcpy(tl->name, name);
	tl->call = call;
	tl->next = tmp;

	return tl;
};

void
test_run(struct test_stat *ts) {
	struct test_list *i = ts->test;
	while (i) {
		struct test_list *garbage = i;
		printf("- Testing %s ", i->name);

		int align = ts->maxlen - strlen(i->name) + 3;
		for (size_t j=0; j<align; ++j) {
			putchar('.');
		}

		putchar(' ');

		if (i->call()) {
			++(ts->failed);
		} else {
			++(ts->passed);
		}

		i = i->next;
		free(garbage);
	}

	ts->test = NULL;

	if (ts->passed || ts->failed) {
		printf("Passed: %d\n", ts->passed);
		printf("Failed: %d\n", ts->failed);
		printf("Total:  %d\n", ts->passed + ts->failed);
	}
}

#define TEST_INIT(handle) struct test_stat handle = {0}

#define TEST_DEFINE(name) int name (void)

#define TEST_END \
	do { \
		printf("pass (\x1B[32mOK\x1B[0m)\n"); \
		return 0; \
	} while (0)

#define TEST_ASSERT(expr) \
	do { \
		if (!(expr)) { \
			printf("fail (\x1B[31m" #expr ":%d\x1B[0m)\n", __LINE__); \
			return -1; \
		} \
	} while (0)

#define TEST_ADD(handle, name) \
	do { \
		(handle).test = test_list_add((handle).test, name, #name); \
		if (strlen(#name) > (handle.maxlen)) { \
			(handle).maxlen = strlen(#name); \
		} \
	} while (0)

#define TEST_RUN(handle) test_run(&(handle))

#endif
