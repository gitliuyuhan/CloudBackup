CC = clang++ -std=c++11
CGLAGS = -g -Wall
LDFLAGS = -pthread
udp:main.o recv_file.o send_file.o tools.o
	${CC} -o udp main.o tools.o  recv_file.o send_file.o ${LDFLAGS}
main.o:
	${CC} -c main.cpp ${LDFLAGS}
tools.o:
	${CC} -c tools.cpp ${LDFLAGS}
recv_file.o:
	${CC} -c recv_file.cpp ${LDFLAGS}
send_file.o:
	${CC} -c send_file.cpp ${LDFLAGS}
.PHONY:clean
clean:
	rm *.o
