.POSIX:

export PLATFORM   ?=qemu_virt
export FLAGS      ?= -finstrument-functions -finstrument-functions-exclude-file-list=${COMMON_INC},tutorial-commons
export ROOT       :=${abspath ../..}
export BUILD      :=${abspath build/${PLATFORM}}
export S3K_CONF_H :=${abspath s3k_conf.h}

include ${ROOT}/tools.mk
include ${ROOT}/common/plat/${PLATFORM}.mk

APPS=app0

ELFS:=${patsubst %, ${BUILD}/%.elf, kernel ${APPS}}

TRACE_SRC:=trace/trace.c
TRACE_OBJ:=${BUILD}/trace.o

all: kernel ${APPS}

clean:
	rm -rf ${BUILD}

common:
	@${MAKE} -C ${ROOT}/common

kernel: common
	@${MAKE} -C ${ROOT}/kernel

${APPS}: common ${TRACE_OBJ}
	@${MAKE} -f ../build.mk PROGRAM=$@ \
		EXTRA_C_FLAGS="${FLAGS}" \
		EXTRA_LD_FLAGS="${TRACE_OBJ} -L${COMMON_LIB} -ls3k -laltc -lplat"

${TRACE_OBJ}: ${TRACE_SRC}
	@mkdir -p ${@D}
	@${CC} -march=${ARCH} -mabi=${ABI} -mcmodel=${CMODEL} \
		-DPLATFORM_${PLATFORM} -nostdlib -Os -g3 -flto \
		-I${COMMON_INC} -include ${S3K_CONF_H} \
		-o $@ -c ${TRACE_SRC}

qemu: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/qemu.sh

qemu-gdb: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/qemu.sh -gdb tcp::3333 -S

gdb: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/gdb.sh

gdb-openocd: kernel ${APPS}
	@ELFS="${ELFS}" ${ROOT}/scripts/gdb-openocd.sh

size: ${ELFS}
	${SIZE} ${ELFS}

.PHONY: all clean size qemu qemu-gdb gdb kernel common ${APPS}
