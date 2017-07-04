#CFLAGS = -g -Wall -Weffc++ -std=c++0x
CFLAGS = -O3 -Wall -Weffc++ -std=c++0x

CC = g++

#INCL = -I../.. -I.. -I/home/nickwebe/boost_1_63_0
INCL = -I../.. -I.. -I/usr/local/boost_for_gcc/include

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
	file_helper.o \
	string_helper.o \
	plot.o \

all : $(OBJ)

args.o : args.cc args.hh ../moer/argbase.hh
	$(CC) -c $(CFLAGS) $(INCL) args.cc

bulk_loader.o : bulk_loader.cc bulk_loader.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) bulk_loader.cc

exception.o : exception.cc exception.hh 
	$(CC) -c $(CFLAGS) $(INCL) exception.cc

print_helper.o : print_helper.cc print_helper.hh common.hh types.hh ../moer/system.hh
	$(CC) -c $(CFLAGS) $(INCL) print_helper.cc

plot.o : plot.cc plot.hh print_helper.hh file_helper.hh string_helper.hh gnuplot-iostream.hh
	$(CC) -c $(CFLAGS) $(INCL) plot.cc

file_helper.o : file_helper.cc file_helper.hh types.hh print_helper.hh string_helper.hh
	$(CC) -c $(CFLAGS) $(INCL) file_helper.cc

string_helper.o : string_helper.cc string_helper.hh
	$(CC) -c $(CFLAGS) $(INCL) string_helper.cc


clean :
	rm -f *.o a.out


