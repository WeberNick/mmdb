#CFLAGS = -g -Wall -Weffc++ -std=c++0x
CFLAGS = -O3 -Wall -Weffc++ -std=c++0x

CC = g++
# CC = g++-4.6

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	../infra/webe/types.hh \
	../infra/webe/common.hh \
	../infra/webe/exception.hh \
	../infra/webe/bulk_loader.hh \
	../infra/moer/datejd.hh \
	../relation/nsm_relation.hh \
	../memory/segment.hh \

OBJ = \
      page_interpreter_sp.o \
      bulk_insert_sp.o \

all : $(OBJ)

page_interpreter_sp.o : page_interpreter_sp.cc page_interpreter_sp.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) page_interpreter_sp.cc

bulk_insert_sp.o : bulk_insert_sp.cc bulk_insert_sp.hh page_interpreter_sp.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) bulk_insert_sp.cc

clean :
	rm -f *.o a.out


