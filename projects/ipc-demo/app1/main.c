#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#include <stdint.h>

#include "../../tutorial-commons/utils.h"

int main(void)
{
	alt_puts("starting user app1");

	// Initializing struct for message and reply from monitor
	s3k_msg_t msg = {0};
	s3k_reply_t reply;
	
	alt_snprintf((char *)&(msg.data), 32, "Return address");
	while (1) {
		for(int i; i < 5; i++){
			do {
				//uint64_t ra = s3k_reg_read(S3K_REG_RA);
				alt_snprintf((char *)&(msg.data), 32, "Return address");
				reply = s3k_sock_sendrecv(APP_1_CAP_SOCKET, &msg);
				if (reply.err == S3K_ERR_TIMEOUT)
					alt_puts("1> timeout");
			} while (reply.err);
			alt_puts((char *)reply.data);
		}	

		for(int i; i < 5; i++){
			do {
				//uint64_t ra = s3k_reg_read(S3K_REG_RA);
				alt_snprintf((char *)&(msg.data), 32, "AAAAAAAAAAAA");
				reply = s3k_sock_sendrecv(APP_1_CAP_SOCKET, &msg);
				if (reply.err == S3K_ERR_TIMEOUT)
					alt_puts("1> timeout");
			} while (reply.err);
			alt_puts((char *)reply.data);
		}	

	}
}
