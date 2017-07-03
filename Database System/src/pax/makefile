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
	../relation/pax_relation.hh \
	../memory/segment.hh \


OBJ = \
	page_interpreter_pax.o \
	bulk_insert_pax.o \

all : $(OBJ)

page_interpreter_pax.o : page_interpreter_pax.cc page_interpreter_pax.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) page_interpreter_pax.cc

bulk_insert_pax.o : bulk_insert_pax.cc bulk_insert_pax.hh page_interpreter_pax.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) bulk_insert_pax.cc

clean :
	rm -f *.o a.out


