/*
MONITOR DEMO

responsible for answering IPC from user-created app. 

*/ 

#include "altc/altio.h"
#include "s3k/s3k.h"
#include "altc/string.h"
#include "../../tutorial-commons/utils.h"


int main(void)
{
	// Setup UART access
	setup_uart();

	// Write hello world.
	alt_printf("DEMO: Starting IPC monitor\n");

	// Setup app1 capabilities and PC
	setup_app_1();

	// Setup scheduling
	setup_scheduling(ROUND_ROBIN);

	uint32_t socket = setup_socket(true, true, false);
	uint32_t free_cap_idx = find_free_cap();

	s3k_mon_resume(MONITOR, APP1_PID);

	// s3k_msg_t msg;

	s3k_msg_t reply = {0};
	s3k_reply_t receive;
	
	while (1) {
		do {
			receive = s3k_sock_recv(socket, free_cap_idx);
			
			if (receive.err == S3K_ERR_TIMEOUT)
				alt_puts("0> timeout");
		} while (receive.err);


		const unsigned char *actual = (const unsigned char *)receive.data;

		alt_printf("FROM APP1: \"%s\" \n", ((char *)receive.data));

		if ((memcmp(receive.data, "Return address", sizeof("Return address")))){

			memcpy(reply.data, "FROM MONITOR: ALLOWED", 32);
			s3k_sock_send(socket, &reply);
		}
		else{
			memcpy(reply.data, "FROM MONITOR: DENIED", 32);
			s3k_sock_send(socket, &reply);
			
			// s3k_cap_revoke(HART0_TIME);
			// debug_capability_from_idx(HART0_TIME);
		}

	}
}




// int memcmp(const void *s1, const void *s2, size_t n);