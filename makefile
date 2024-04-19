CC=g++ 
CCFLAGS=-Wall -Wextra -Wold-style-cast -Werror -fexec-charset=utf-8 
INC=
LIN=-lsqlite3
DEF=
STD=-std=c++17

src=src/util.cc src/main.cc src/chart.cc src/index.cc src/project.cc
exe=test.exe
prexe=arcpkg-rd.exe

all: com 

dbg: dbgc 

prod: $(src) 
	rm -f $(prexe) 
	$(CC) $(CCFLAGS) -o$(prexe) $(src) $(INC) $(LIN) $(DEF) $(STD) -O3

com: $(src)
	rm -f $(exe)
	$(CC) $(CCFLAGS) -o$(exe) $(src) $(INC) $(LIN) $(DEF) $(STD) -O0

dbgc: $(src) 
	rm -f $(exe)
	$(CC) $(CCFLAGS) -o$(exe) $(src) $(INC) $(LIN) $(DEF) $(STD) -g3 

run: $(exe)
	./$(exe)

dbgr: $(exe)
	gdb $(exe)

clear: 
	rm -rf build/*