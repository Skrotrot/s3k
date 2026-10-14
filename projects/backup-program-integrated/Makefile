.POSIX:

export PLATFORM   		?=qemu_virt4
export ROOT       		:=${abspath ../..}
export BUILD      		:=${abspath build/${PLATFORM}}
export S3K_CONF_H 		:=${abspath s3k_conf.h}
export EXTRA_CFLAGS 	:= -O0 -fno-lto -fno-inline -fno-optimize-sibling-calls
export EXTRA_LDFLAGS 	:= -fno-lto

include ${ROOT}/tools.mk

APPS=app0 app1

ELFS:=${patsubst %, ${BUILD}/%.elf, kernel ${APPS}}

all: kernel ${APPS}

clean:
	rm -rf ${BUILD}


common:
	@${MAKE} -C ${ROOT}/common

app1: common
	@${MAKE} -f ../build.mk PROGRAM=app1 

app1-postprocess: app1
	echo ${BUILD}
	readelf -s ${BUILD}/app1.elf | awk '$$8 == "_start" {print "#define app1_start 0x" $$2}' > app_sections.h
	readelf -s ${BUILD}/app1.elf | awk '$$8 == "_data" {print "#define app1_data 0x" $$2}' >> app_sections.h
	readelf -s ${BUILD}/app1.elf | awk '$$8 == "_bss" {print "#define app1_bss 0x" $$2}' >> app_sections.h
	readelf -s ${BUILD}/app1.elf | awk '$$8 == "_end" {print "#define app1_end 0x" $$2}' >> app_sections.h

	readelf -s ${BUILD}/app1.elf | awk '$$8 == "__stack_pointer" {print "#define app1_stack_pointer 0x" $$2}' >> app_sections.h
	readelf -s ${BUILD}/app1.elf | awk '$$8 == "__stack_size" {print "#define app1_Stack_size 0x" $$2}' >> app_sections.h
kernel: common 
	@${MAKE} -C ${ROOT}/kernel

app0: common app1-postprocess
	@${MAKE} -f ../build.mk PROGRAM=app0 

qemu: kernel app1-postprocess app0 
	@ELFS="${ELFS}" ${ROOT}/scripts/qemu.sh

qemu-gdb: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/qemu.sh -gdb tcp::3333 -S

gdb: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/gdb.sh

gdb-openocd: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/gdb-openocd.sh

size: ${ELFS}
	${SIZE} ${ELFS}

.PHONY: all clean size qemu qemu-gdb gdb kernel common app1-postprocess ${APPS}





