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

	alt_printf("DEMO: Starting IPC monitor\n");

	// Setup app1 capabilities and PC
	setup_app_1();

	// Setup scheduling
	setup_scheduling(ROUND_ROBIN);

	
	uint32_t socket = setup_socket(true, true, false);
	uint32_t free_cap_idx = find_free_cap();

	s3k_mon_resume(MONITOR, APP1_PID);

	s3k_msg_t reply = {0};
	s3k_reply_t receive;
	s3k_state_t state;
	s3k_err_t err;
	const char *expected = "Return address";
	//debg 

	int running = 1;
	while (running) {
		receive = s3k_sock_recv(socket, free_cap_idx);
		alt_printf("FROM APP1: \"%s\" \n", ((char *)receive.data));

		if ((memcmp(receive.data, expected, sizeof(expected)))){

			memcpy(reply.data, "FROM MONITOR: ALLOWED", 32);
			s3k_sock_send(socket, &reply);
		}
		else
		{
			memcpy(reply.data, "FROM MONITOR: DENIED", 32);
			s3k_sock_send(socket, &reply);

			// There are mulitple ways to remove capabilites from app1
				//s3k_cap_delete(HART1_TIME);
				//s3k_cap_revoke(HART0_TIME);

			s3k_mon_suspend(MONITOR, APP1_PID);

			err = s3k_mon_state_get(MONITOR, APP1_PID, &state);
			if (err == S3K_SUCCESS)
			{
				alt_printf("app state: 0x%x\n", state);
			}
		}
	}
	alt_printf("program stopped!\n");
}
