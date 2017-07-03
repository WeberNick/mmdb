#CFLAGS = -g -Wall -Weffc++ -std=c++0x
#CFLAGS = -O3 -Wall -Weffc++ -std=c++0x
CFLAGS = -O3 -std=c++0x

CC = g++
# CC = g++-4.6

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
	$(CC) -c $(CFLAGS) $(INCL) datejd.cc

linereader.o : linereader.cc linereader.hh datejd.hh decimal.hh val_tt.hh stime.hh stimestamp.hh
	$(CC) -c $(CFLAGS) $(INCL) linereader.cc


simplecharcontainer.o : simplecharcontainer.cc simplecharcontainer.hh
	$(CC) -c $(CFLAGS) $(INCL) simplecharcontainer.cc

simpleexception.o : simpleexception.cc simpleexception.hh
	$(CC) -c $(CFLAGS) $(INCL) simpleexception.cc

stime.o : stime.cc stime.hh
	$(CC) -c $(CFLAGS) $(INCL) stime.cc

stimestamp.o : stimestamp.cc stimestamp.hh datejd.hh stime.hh
	$(CC) -c $(CFLAGS) $(INCL) stimestamp.cc

system.o : system.cc system.hh
	$(CC) -c $(CFLAGS) $(INCL) system.cc

memory_access.o : memory_access.cc memory_access.hh measure.hh random_permutation.hh ../webe/types.hh ../webe/print_helper.hh
	$(CC) -c $(CFLAGS) $(INCL) memory_access.cc

clean :
	rm -f *.o a.out


