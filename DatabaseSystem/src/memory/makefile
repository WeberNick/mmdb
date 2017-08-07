#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -Wall -Weffc++ -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

OBJ = \
	memory_manager.o \
	segment.o \


all : $(OBJ)

memory_manager.o : memory_manager.cc memory_manager.hh ../infra/webe/types.hh ../infra/webe/common.hh ../infra/webe/exception.hh
	$(CC) -c $(CXXFLAGS) $(INCL) memory_manager.cc

segment.o : segment.cc segment.hh ../infra/webe/types.hh
	$(CC) -c $(CXXFLAGS) $(INCL) segment.cc

clean :
	rm -f *.o a.out


