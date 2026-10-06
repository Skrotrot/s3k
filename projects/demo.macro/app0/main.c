#include "altc/altio.h"
#include "s3k/s3k.h"
#include "../../tutorial-commons/utils.h"

static int _call_depth = 0;

#define CFI_ENTER() \
    do { \
        alt_printf("Enter %d   ", ++_call_depth); \
        alt_printf("0x%x -\n", __builtin_return_address(0)); \
    } while (0)

#define CFI_RETURN(...) \
    do { \
        alt_printf("Exit  %d   ", _call_depth--); \
        alt_printf("           - 0x%x\n", __builtin_return_address(0)); \
        return __VA_ARGS__; \
    } while (0)

void test(void);
void test1(void);
void test2(void);
void test3(void);

void test(void) { 
    CFI_ENTER();
    test1();
    test2();
    CFI_RETURN();
}

void test1() {
    CFI_ENTER();
    test2();
    test3();
    CFI_RETURN();
}

void test2(void) {
    CFI_ENTER();
    test3();
    CFI_RETURN();
}

void test3(void) {
    CFI_ENTER();
    CFI_RETURN();
}

int main(void) {
	setup_uart();

	test();
}
