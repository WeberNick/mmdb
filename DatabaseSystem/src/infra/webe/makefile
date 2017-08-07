#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	types.hh \
	common.hh \
	exception.hh \
	print_helper.hh \
	../../relation/relation.hh \
	../moer/linereader.hh \
	../moer/simplecharcontainer.hh \
	../moer/datejd.hh \

OBJ = \
	args.o \
	exception.o \
	bulk_loader.o \
	print_helper.o \

all : $(OBJ)

args.o : args.cc args.hh ../moer/argbase.hh
	$(CC) -c $(CXXFLAGS) $(INCL) args.cc

bulk_loader.o : bulk_loader.cc bulk_loader.hh $(HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) bulk_loader.cc

exception.o : exception.cc exception.hh 
	$(CC) -c $(CXXFLAGS) $(INCL) exception.cc

print_helper.o : print_helper.cc print_helper.hh common.hh types.hh ../moer/system.hh
	$(CC) -c $(CXXFLAGS) $(INCL) print_helper.cc

clean :
	rm -f *.o a.out


