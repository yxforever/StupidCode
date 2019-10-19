CNDTN=error.o readline.o readn.o writen.o wrapstdio.o wrapsock.o
OBJS = main.o icmpcode_v4.o recv_v4.o sig_alrm.o traceloop.o tv_sub.o host_serv.o sock_ntop_host.o signal.o sock_cmp_addr.o sock_set_port.c

traceroute:	${OBJS} $(CNDTN)
		gcc ${OBJS} $(CNDTN) -o traceroute
%.o:%.c 
	gcc -c -o $@ $<
clean:
	rm -f *.o
