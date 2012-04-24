.PHONY: clean all

INCS = -I/usr/X11R6/include
LIBS = -lX11 -L/usr/X11R6/lib

CFLAGS += -std=c99 -pedantic -Wall ${INCS}
LDFLAGS += ${LIBS}

CC? = cc


options:
	@echo build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC		   = ${CC}"

status_freebsd: status_freebsd.c
	@echo CC -o $@
	@${CC} -o dwm_status ${CFLAGS} ${LDFLAGS} $<

status_linux: status_linux.c
	@echo CC -o $@
	@${CC} -o dwm_status ${CFLAGS} ${LDFLAGS} -lm $<

install:
	cp dwm_status ${HOME}/bin

clean:
	@rm -f dwm_status *.o
