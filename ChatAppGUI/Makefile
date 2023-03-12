
PROG=myapp

CC=gcc
LIBS= `pkg-config --libs gtk+-3.0`
CFLAGS=`pkg-config --cflags gtk+-3.0`



${PROG}: ${PROG}.c
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.c ${LIBS}

clean:
	rm ${PROG}
