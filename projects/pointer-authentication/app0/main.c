#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "forward_edge.h"


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

// This function is to demonstrate pointer authentication prevents exploitation of vulnerable function.
void vulnerable_forward_edge_protected(void)
{
    struct forward_edge_data_1 frame = {.callback = PTR_AUTH_INIT(forward_edge_target)};
    struct forward_edge_data_2 payload = {.callback = PTR_AUTH_INIT(win2)};

    alt_printf("Inside vulnerable forward-edge protected\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Overflow the buffer into callback, redirecting the indirect call to win2().
    memcpy(frame.buffer, &payload, sizeof(payload));

    // Signature should fail based on wrong type detected.
    PTR_AUTH(frame.callback);
}

// This function is to demonstrate the vulnerability exists without pointer authentication.
void vulnerable_forward_edge(void)
{
    struct forward_edge_data_1 frame = {.callback = {.callback_fun = forward_edge_target}};
    struct forward_edge_data_2 payload = {.callback = {.callback_fun = win2}};

    alt_printf("Inside vulnerable forward-edge\n");

    memset(payload.buffer, 'A', sizeof(payload.buffer));

    // Overflow the buffer into callback, redirecting the indirect call to win2().
    memcpy(frame.buffer, &payload, sizeof(payload));

    // No authentication, win2 should run. 
    frame.callback.callback_fun();
}


// This function is to demonstrate the pointer authentication will not break functions in normal (non-vulnerable) cases.
int non_vulnerable_forward_edge(void)
{
    struct forward_edge_data_1 frame = {.callback = PTR_AUTH_INIT(forward_edge_target)};
    struct forward_edge_data_2 payload = {.callback = PTR_AUTH_INIT(win2)};

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

    vulnerable_forward_edge();

    //alt_printf("Successful exit\n");
}
