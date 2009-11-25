PREFIX=/usr
DESTDIR=
CC=gcc

xrectsel : xrectsel.c -lX11
	${CC} ${CFLAGS} xrectsel.c -lX11 -o xrectsel

install : xrectsel ffcast ffcast.1.gz
	install -D -m755 xrectsel ${DESTDIR}${PREFIX}/bin/xrectsel
	install -D -m755 ffcast ${DESTDIR}${PREFIX}/bin/ffcast
	install -D -m644 man/ffcast.1.gz ${DESTDIR}${PREFIX}/share/man/man1/ffcast.1.gz

.PHONY : clean
clean :
	-rm xrectsel
