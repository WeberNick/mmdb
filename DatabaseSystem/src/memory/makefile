#CXXFLAGS = -g -Wall -Weffc++ -std=c++11
CXXFLAGS = -O3 -Wall -Weffc++ -std=c++11

CC = g++

INCL = -I../.. -I..

HDR = \
	../infra/webe/types.hh \
	../infra/webe/exception.hh \
	\

OBJ = \
	memory_manager.o \
	segment.o \


all : $(OBJ)

memory_manager.o : memory_manager.cc memory_manager.hh $(HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) memory_manager.cc

segment.o : segment.cc segment.hh $(HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) segment.cc

clean :
	rm -f *.o a.out


