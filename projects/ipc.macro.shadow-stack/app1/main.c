#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "backward_edge.h"
#include "forward_edge.h"

#define BUF_OFFSET 128
static int _call_depth = 0;

void win(void)
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
struct forward_edge_data_1 {
    char buffer[32];
    struct function_ptr_1 callback;
};

struct forward_edge_data_2 {
    char buffer[32];
    struct function_ptr_2 callback;
};


static void forward_edge_target(void)
{
    alt_printf("Expected forward-edge target\n");
}

static void win2(int a, int b)
{
    alt_printf("Attack 2 works");
}

void vulnerable_forward_edge_protected(void)
{
    struct forward_edge_data_1 frame = {.callback = PTR_AUTH_INIT(forward_edge_target)};
    struct forward_edge_data_2 payload = {.callback = PTR_AUTH_INIT(win2)};

    alt_printf("Inside vulnerable forward-edge protected\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Overflow the buffer into callback, redirecting the indirect call to win().
    memcpy(frame.buffer, &payload, sizeof(payload));

    PTR_AUTH(frame.callback);
}

void vulnerable_forward_edge(void)
{
    struct forward_edge_data_1 frame = {.callback = {.callback_fun = forward_edge_target}};
    struct forward_edge_data_2 payload = {.callback = {.callback_fun = win2}};

    alt_printf("Inside vulnerable forward-edge\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Overflow the buffer into callback, redirecting the indirect call to win().
    memcpy(frame.buffer, &payload, sizeof(payload));

    frame.callback.callback_fun();
}


int non_vulnerable_forward_edge(void)
{
    struct forward_edge_data_1 frame = {.callback = PTR_AUTH_INIT(forward_edge_target)};
    struct forward_edge_data_1 payload = {.callback = PTR_AUTH_INIT(win)};

    alt_printf("Inside non-vulnerable forward-edge\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Limit the same attack payload to the buffer, preserving callback.
    memcpy(frame.buffer, &payload, sizeof(frame.buffer));

    PTR_AUTH(frame.callback);

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

    alt_printf("Before Vulnerable Forward-Edge\n");

    //vulnerable_forward_edge();

    //alt_printf("Successful exit\n");
}
