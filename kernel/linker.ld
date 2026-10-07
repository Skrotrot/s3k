/* See LICENSE file for copyright and license details. */
OUTPUT_ARCH(riscv)
ENTRY(_start)

_payload = ORIGIN(RAM) + LENGTH(RAM);

__global_pointer$ = MIN(_sdata + 0x800, MAX(_data + 0x800, _end - 0x800));

S3K_PROC_CNT = 2;
S3K_REG_SIZE = 0x138;
S3K_PROC_MEM_SIZE = 0x10000;
BACKUP_SIZE = S3K_PROC_CNT * (S3K_REG_SIZE + S3K_PROC_MEM_SIZE);
RAM_TOP = 0x88000000;

MEMORY {
	RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 64K
	BACKUP (rw) : ORIGIN = RAM_TOP - BACKUP_SIZE, LENGTH = BACKUP_SIZE
}

SECTIONS {
	.text : {
		*( .text.init )
		*( .text .text.* )
	} > RAM

	.data : ALIGN(4K) {
		_data = . ;
		*( .data )
		*( .data.* )
		_sdata = . ;
		*( .sdata )
		*( .sdata.* )
	} > RAM

	.bss : ALIGN(4K) {
		_bss = .;
		_sbss = .;
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		_end = ALIGN(8);
	} > RAM

	.backup : ALIGN(8) {
		_backup = .;
		. += BACKUP_SIZE;
		_backup_end = .;
	} > BACKUP
}
