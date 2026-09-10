#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"

void win()
{
  alt_printf("Attack works\n");
}

static char source[24];

void vulnerable(void)
{
  char buffer[2];

  alt_printf("Inside\n");

  memset(source, 'A', 16); //Offset to RA

  *(uint64_t *)&source[16] = (uint64_t)win; // Overwrite RA with win()

  memcpy(buffer, source, sizeof(source) + 64);
}

int main(void)
{
	// Setup UART access
  setup_uart();

  alt_printf("Before\n");

  vulnerable();

  alt_printf("Successful exit\n");
}



