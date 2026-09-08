#include "altc/altio.h"
#include "s3k/s3k.h"
#include "../../tutorial-commons/utils.h"

void test(void);
void test1(void);
void test2(void);
void test3(void);

void test(void) { 
    test1();
    test2();
}

void test1() {
    test2();
    test3();
}

void test2(void) {
    test3();
}

void test3(void) {}

__attribute__((no_instrument_function))
int main(void) {
	setup_uart();

	test();
}
