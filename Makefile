PREFIX=/usr
DESTDIR=
CC=gcc

xrectsel : xrectsel.c -lX11
	${CC} ${CFLAGS} xrectsel.c -lX11 -o xrectsel

install : xrectsel ffcast
	install -D -m755 xrectsel ${DESTDIR}${PREFIX}/bin/xrectsel
	install -D -m755 ffcast ${DESTDIR}${PREFIX}/bin/ffcast

.PHONY : clean
clean :
	-rm xrectsel
