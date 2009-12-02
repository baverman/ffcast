PREFIX=/usr
DESTDIR=
CC=gcc

xrectsel : xrectsel.c -lX11
	${CC} ${CFLAGS} xrectsel.c -lX11 -o xrectsel

install : xrectsel ffcast ffcast.conf ffcast.1
	install -D -m755 xrectsel ${DESTDIR}${PREFIX}/bin/xrectsel
	install -D -m755 ffcast ${DESTDIR}${PREFIX}/bin/ffcast
	install -D -m644 ffcast.conf ${DESTDIR}${PREFIX}/share/ffcast/doc/ffcast.conf
	install -D -m644 ffcast.1 ${DESTDIR}${PREFIX}/share/man/man1/ffcast.1

.PHONY : clean
clean :
	-rm xrectsel
