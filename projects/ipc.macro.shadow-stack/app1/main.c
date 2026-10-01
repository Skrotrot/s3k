#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#define BUF_OFFSET 128
static int _call_depth = 0;

#define CFI_ENTER() \
    s3k_msg_t msg = {0}; \
    void *__cfi_ra; \
    __asm__ volatile ("mv %0, ra" : "=r"(__cfi_ra)); \
    msg.data[0] = (uint64_t)__cfi_ra; \
    msg.data[1] = (uint64_t)1; /* msg.data[1] set to 1 in order to distinguish between Enter or Exit. */ \
    s3k_sock_sendrecv(APP_1_CAP_SOCKET, &msg); \
   

#define CFI_RETURN(...) \
    do { \
        s3k_msg_t msg = {0}; \
        void *__cfi_ra_now; \
        __asm__ volatile ("ld %0, -8(s0)" : "=r"(__cfi_ra_now)); \
        msg.data[0] = (uint64_t)__cfi_ra_now; \
        msg.data[1] = (uint64_t)0; \
        s3k_sock_sendrecv(APP_1_CAP_SOCKET, &msg); \
        return __VA_ARGS__; \
    } while (0)


#define PTR_CRYPT(ptr) \
    uint64_t key = 18446744073709551615ULL; \
    ptr = (void*)((uint64_t)(ptr) ^ key); \


#define PTR_DECRYPT(ptr) \
    ptr = (void*)((uint64_t)(ptr) ^ key); \

void win()
{
    alt_printf("Attack Works\n");
    do {} while(1);
}

static char source[BUF_OFFSET];
 
void vulnerable(void)
{
    CFI_ENTER();
    char buffer[2];

    alt_printf("Inside vulnearble\n");

    memset(source, 'A', BUF_OFFSET); // Offset to RA, changed due to larger RETURN macro.

    *(uint64_t *)&source[BUF_OFFSET - 8] = (uint64_t)win; // Overwrite RA with win() (Will succeed)

    memcpy(buffer, source, sizeof(source));

    CFI_RETURN();
}

int non_vulnerable(void)
{
    CFI_ENTER();
    char buffer[128];

    alt_printf("Inside non-vulnerable\n");
  
    memset(source, 'A', 32); // Offset to RA

    *(uint64_t *)&source[32] = (uint64_t)win; // Try to overwrite RA with win() (Will fail)
  
    memcpy(buffer, source, sizeof(source)); 

    CFI_RETURN(1);
}

// Keep the buffer and indirect-call target together so the overflow reaches it.
struct forward_edge_data {
    char buffer[32];
    void (*callback)(void);
};

static void forward_edge_target(void)
{
    alt_printf("Expected forward-edge target\n");
}

void vulnerable_forward_edge_protected(void)
{
    struct forward_edge_data frame = {.callback = forward_edge_target};
    PTR_CRYPT(frame.callback);
    struct forward_edge_data payload = {.callback = win};

    alt_printf("Inside vulnerable forward-edge protected\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Overflow the buffer into callback, redirecting the indirect call to win().
    memcpy(frame.buffer, &payload, sizeof(payload));

    PTR_DECRYPT(frame.callback);
    frame.callback();
}

void vulnerable_forward_edge(void)
{
    struct forward_edge_data frame = {.callback = forward_edge_target};
    struct forward_edge_data payload = {.callback = win};

    alt_printf("Inside vulnerable forward-edge\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Overflow the buffer into callback, redirecting the indirect call to win().
    memcpy(frame.buffer, &payload, sizeof(payload));

    frame.callback();
}


int non_vulnerable_forward_edge(void)
{
    struct forward_edge_data frame = {.callback = forward_edge_target};
    PTR_CRYPT(frame.callback);
    struct forward_edge_data payload = {.callback = win};

    alt_printf("Inside non-vulnerable forward-edge\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Limit the same attack payload to the buffer, preserving callback.
    memcpy(frame.buffer, &payload, sizeof(frame.buffer));

    PTR_DECRYPT(frame.callback);
    frame.callback();

    return 1;
}

int main(void)
{
    // Setup UART access
    setup_uart();

    alt_printf("Before Non-Vulnerable Forward-Edge\n");

    non_vulnerable_forward_edge();

    alt_printf("Before Vulnerable Forward-Edge Protected\n");

    vulnerable_forward_edge_protected();

    //alt_printf("Before Vulnerable Forward-Edge\n");

    //vulnerable_forward_edge();

    alt_printf("Successful exit\n");
}
