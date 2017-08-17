CXXFLAGS = -O3 -std=c++0x

CC = g++

# INCL = -I../.. -I.. -I/home/nickwebe/boost_1_63_0
INCL = -I../.. -I.. -I/usr/local/boost_for_gcc/include/boost

VPATH = .:..:../..

OBJ = \
	file_helper.o \
	string_helper.o \
	plot.o \

ALL = main 


all : $(ALL)

# main: main.o $(OBJ)
# 	$(CC) -o plot main.o $(OBJ) -lboost_iostreams -lboost_system -lboost_filesystem

main: main.o $(OBJ)
	$(CC) -o plot main.o $(OBJ) -L/usr/local/boost_for_gcc/lib -lboost_iostreams -lboost_system -lboost_filesystem

main.o : main.cc plot.hh
	$(CC) -c $(CXXFLAGS) $(INCL) main.cc

file_helper.o : file_helper.cc file_helper.hh string_helper.hh
	$(CC) -c $(CXXFLAGS) $(INCL) file_helper.cc

string_helper.o : string_helper.cc string_helper.hh
	$(CC) -c $(CXXFLAGS) $(INCL) string_helper.cc

plot.o : plot.cc plot.hh file_helper.hh string_helper.hh gnuplot-iostream.hh
	$(CC) -c $(CXXFLAGS) $(INCL) plot.cc


clean :
	rm -f *.o a.out


