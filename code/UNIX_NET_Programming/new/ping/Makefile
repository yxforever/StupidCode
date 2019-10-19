CNDTN=error.o readline.o readn.o writen.o wrapstdio.o wrapsock.o
OBJS = main.o proc_v4.o readloop.o send_v4.o sig_alrm.o tv_sub.o in_cksum.o host_serv.o sock_ntop_host.o signal.o

ping:	${OBJS} $(CNDTN)
		gcc ${OBJS} $(CNDTN) -o ping
%.o:%.c
	gcc -c -o $@ $<
clean:
	rm -f *.o 


