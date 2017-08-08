#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -Wall -Weffc++ -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

HDRPI = \
	../infra/webe/types.hh \

HDRBI = \
	../infra/webe/types.hh \
	../infra/webe/exception.hh \
	../infra/webe/intermediate_buffer.hh \
	../infra/webe/bulk_loader.hh \
	../infra/moer/datejd.hh \
	../relation/pax_relation.hh \
	../memory/segment.hh \
	page_interpreter_pax.hh \


OBJ = \
	page_interpreter_pax.o \
	bulk_insert_pax.o \

all : $(OBJ)

page_interpreter_pax.o : page_interpreter_pax.cc page_interpreter_pax.hh $(HDRPI)
	$(CC) -c $(CXXFLAGS) $(INCL) page_interpreter_pax.cc

bulk_insert_pax.o : bulk_insert_pax.cc bulk_insert_pax.hh $(HDRBI)
	$(CC) -c $(CXXFLAGS) $(INCL) bulk_insert_pax.cc

clean :
	rm -f *.o a.out


