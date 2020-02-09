#保存 .o 文件
objects =  cerrexit.o connectsock.o connectTCP.o connectUDP.o \
	dcon.o do_echo.o do_noga.o do_notsup.o do_txbinary.o \
	errexit.o fsmbuild.o fsminit.o passivesock.o recopt.o \
	scrgetc.o scrinit.o scrwrap.o sofsm.o sonotsup.o soputc.o \
	sowrite.o status.o subend.o subfsm.o subopt.o subtermtype.o \
	suspend.o sync.o tclient.o tcout.o telnet.o tnabort.o ttfsm.o \
	ttputc.o ttwrite.o ttysetup.o unscript.o will_notsup.o \
	will_termtype.o will_txbinary.o xput.o
	
telnet: cerrexit.o connectsock.o connectTCP.o connectUDP.o \
	dcon.o do_echo.o do_noga.o do_notsup.o do_txbinary.o \
	errexit.o fsmbuild.o fsminit.o passivesock.o recopt.o \
	scrgetc.o scrinit.o scrwrap.o sofsm.o sonotsup.o soputc.o \
	sowrite.o status.o subend.o subfsm.o subopt.o subtermtype.o \
	suspend.o sync.o tclient.o tcout.o telnet.o tnabort.o ttfsm.o \
	ttputc.o ttwrite.o ttysetup.o unscript.o will_notsup.o \
	will_termtype.o will_txbinary.o xput.o
	gcccerrexit.o connectsock.o connectTCP.o connectUDP.o \
	dcon.o do_echo.o do_noga.o do_notsup.o do_txbinary.o \
	errexit.o fsmbuild.o fsminit.o passivesock.o recopt.o \
	scrgetc.o scrinit.o scrwrap.o sofsm.o sonotsup.o soputc.o \
	sowrite.o status.o subend.o subfsm.o subopt.o subtermtype.o \
	suspend.o sync.o tclient.o tcout.o telnet.o tnabort.o ttfsm.o \
	ttputc.o ttwrite.o ttysetup.o unscript.o will_notsup.o \
	will_termtype.o will_txbinary.o xput.o -o telnet
	
cerrexit.o: cerrexit.c
	gcc -c cerrexit.c -o cerrexit.o
connectsock.o: connectsock.c
	gcc -c connectsock.c -o connectsock.o
connectTCP.o: connectTCP.c
	gcc -c connectTCP.c -o connectTCP.o
connectUDP.o: connectUDP.c
	gcc -c connectUDP.c -o connectUDP.o 
dcon.o: dcon.c
	gcc -c dcon.c -o dcon.o 
do_echo.o: do_echo.c 
	gcc -c do_echo.c -o do_echo.o 
do_noga.o: do_noga.c 
	gcc -c do_noga.c -o do_noga.o 
do_notsup.o: do_notsup.c 
	gcc -c do_notsup.c -o do_notsup.o 
do_txbinary.o: do_txbinary.c 
	gcc -c do_txbinary.c -o do_txbinary.o 
errexit.o: errexit.c 
	gcc -c errexit.c -o errexit.o 
fsmbuild.o: fsmbuild.c 
	gcc -c fsmbuild.c -o fsmbuild.o 
fsminit.o: fsminit.c 
	gcc -c fsminit.c -o fsminit.o 
passivesock.o: passivesock.c 
	gcc -c passivesock.c -o passivesock.o 
recopt.o: recopt.c 
	gcc -c recopt.c -o recopt.o 
scrgetc.o: scrgetc.c 
	gcc -c scrgetc.c -o scrgetc.o 
scrinit.o: scrinit.c 
	gcc -c scrinit.c -o scrinit.o 
scrwrap.o:scrwrap.c 
	gcc -c scrwrap.c -o scrwrap.o 
sofsm.o: sofsm.c 
	gcc -c sofsm.c -o sofsm.o 
sonotsup.o: sonotsup.c 
	gcc -c sonotsup.c -o sonotsup.o 
soputc.o: soputc.c 
	gcc -c soputc.c -o soputc.o 
sowrite.o: sowrite.c 
	gcc -c sowrite.c -o sowrite.o 
status.o: status.c 
	gcc -c status.c -o status.o 
subend.o: subend.c 
	gcc -c subend.c -o subend.o 
subfsm.o: subfsm.c 
	gcc -c subfsm.c -o subfsm.o
subopt.o: subopt.c 
	gcc -c subopt.c -o subopt.o 
subtermtype.o: subtermtype.c 
	gcc -c subtermtype.c -o subtermtype.o 
suspend.o: suspend.c 
	gcc -c suspend.c -o suspend.o 
sync.o: sync.o 
	gcc -c sync.c -o sync.o 
tclient.o: tclient.c 
	gcc -c tclient.c -o tclient.o 
tcout.o: tcout.c 
	gcc -c tcout.c -o tcout.o 
telnet.o: telnet.c 
	gcc -c telnet.c -o telnet.o 
tnabort.o: tnabort.c 
	gcc -c tnabort.c -o tnabort.o 
ttfsm.o: ttfsm.c 
	gcc -c ttfsm.c -o ttfsm.o 
ttputc.o: ttputc.c 
	gcc -c ttputc.c -o ttputc.o 
ttwrite.o: ttwrite.c 
	gcc -c ttwrite.c -o ttwrite.o 
ttysetup.o: ttysetup.c 
	gcc -c ttysetup.c -o ttysetup.o 
unscript.o: unscript.c 
	gcc -c unscript.c -o unscript.o 
will_notsup.o: will_notsup.c 
	gcc -c will_notsup.c -o will_notsup.o 
will_termtype.o: will_termtype.c 
	gcc -c will_termtype.c -o will_termtype.o 
will_txbinary.o: will_txbinary.c 
	gcc -c will_txbinary.c -o will_txbinary.o 
xput.o: xput.c
	gcc -c xput.c -o xput.o 
	
	
#清除标签
clean:
	rm -rf *.o telnet
	
