#CXXFLAGS = -g -Wall -Weffc++ -std=c++11
CXXFLAGS = -O3 -Wall -Weffc++ -std=c++11

CC = g++

INCL = -I../.. -I..

HDRPI = \
	../infra/webe/types.hh \

HDRBI = \
	../infra/webe/types.hh \
	../infra/webe/exception.hh \
	../infra/webe/intermediate_buffer.hh \
	../infra/webe/bulk_loader.hh \
	../infra/moer/datejd.hh \
	../relation/nsm_relation.hh \
	../memory/segment.hh \
	page_interpreter_sp.hh \

OBJ = \
      page_interpreter_sp.o \
      bulk_insert_sp.o \

all : $(OBJ)

page_interpreter_sp.o : page_interpreter_sp.cc page_interpreter_sp.hh $(HDRPI)
	$(CC) -c $(CXXFLAGS) $(INCL) page_interpreter_sp.cc

bulk_insert_sp.o : bulk_insert_sp.cc bulk_insert_sp.hh $(HDRBI)
	$(CC) -c $(CXXFLAGS) $(INCL) bulk_insert_sp.cc

clean :
	rm -f *.o a.out


