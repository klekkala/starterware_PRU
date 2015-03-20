C_DIR := /usr/share/ti/cgt-pru/include
A_DIR := /usr/share/ti/cgt-pru/lib
SWDIR = /usr/share/pru_am335x_starterware
INCLUDEDIR = -I$(C_DIR) -I$(SWDIR)/include  -I$(SWDIR)/include/hw
LIBDIR = -i$(SWDIR)/lib -i$(A_DIR)

all: pru0_firmware pru1_firmware

CFLAGS := -03 --c99 $(INCLUDEDIR)

#PRU0
pru0_firmware: pru0_firmware.obj pru0_syscall.obj AM3359_PRU.cmd
	lnkpru AM3359_PRU.cmd -o=pru0_firmware pru0_firmware.obj pru0_syscall.obj $(LIBDIR) -llibstarterware.a

pru0_firmware.obj: pru0_firmware.c pru0_firmware.h pru_firmware.h
	clpru $(CFLAGS) pru0_firmware.c 

pru0_syscall.obj: pru0_syscall.asm
	clpru pru0_syscall.asm

#PRU1
pru1_firmware: pru1_firmware.obj AM3359_PRU.cmd
	lnkpru -i$(A_DIR) pru1_firmware.obj  AM3359_PRU.cmd -o=pru1_firmware

pru1_firmware.obj: pru1_firmware.c pru1_firmware.h pru_firmware.h
	clpru -O3  --c99 pru1_firmware.c 


install: all
	install -m 444 pru0_firmware /lib/firmware
	install -m 444 pru1_firmware /lib/firmware
