CCX 		= g++
CCINCLUDE	=
CCFLAGS		= -std=c++11 -lnsl
LDFLAGS		=
LIBS		= Restart.h TCPSocket.h TicTacToe.h TicTacToeConstants.h StringTrim.h PrintInColor.h
PROGNAME	= lab5
OBJS		= lab5.o Restart.o TCPSocket.o TicTacToe.o
HDRS		=


.cc.o:
	@echo "Compiling " $<
	$(CCX) -c $(CCFLAGS) $(CCINCLUDE) $<
	@echo $@ "done"

all:	$(PROGNAME)

lab5: $(OBJS)
	$(CCX) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f a.out core $(PROGNAME) $(OBJS)

