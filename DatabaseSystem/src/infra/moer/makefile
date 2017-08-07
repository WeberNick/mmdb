#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
#CXXFLAGS = -O3 -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

OBJ = \
      datejd.o \
      linereader.o \
      simplecharcontainer.o \
      simpleexception.o \
      stime.o \
      stimestamp.o \
      system.o \
      memory_access.o \

all : $(OBJ)

datejd.o : datejd.cc datejd.hh
	$(CC) -c $(CXXFLAGS) $(INCL) datejd.cc

linereader.o : linereader.cc linereader.hh datejd.hh decimal.hh val_tt.hh stime.hh stimestamp.hh
	$(CC) -c $(CXXFLAGS) $(INCL) linereader.cc


simplecharcontainer.o : simplecharcontainer.cc simplecharcontainer.hh
	$(CC) -c $(CXXFLAGS) $(INCL) simplecharcontainer.cc

simpleexception.o : simpleexception.cc simpleexception.hh
	$(CC) -c $(CXXFLAGS) $(INCL) simpleexception.cc

stime.o : stime.cc stime.hh
	$(CC) -c $(CXXFLAGS) $(INCL) stime.cc

stimestamp.o : stimestamp.cc stimestamp.hh datejd.hh stime.hh
	$(CC) -c $(CXXFLAGS) $(INCL) stimestamp.cc

system.o : system.cc system.hh
	$(CC) -c $(CXXFLAGS) $(INCL) system.cc

memory_access.o : memory_access.cc memory_access.hh measure.hh random_permutation.hh ../webe/types.hh ../webe/print_helper.hh
	$(CC) -c $(CXXFLAGS) $(INCL) memory_access.cc

clean :
	rm -f *.o a.out


