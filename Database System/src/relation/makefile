#CFLAGS = -g -Wall -Weffc++ -std=c++0x
CFLAGS = -O3 -Wall -Weffc++ -std=c++0x

CC = g++
# CC = g++-4.6

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	../infra/webe/types.hh \
	../infra/webe/exception.hh \
	../infra/webe/print_helper.hh \
	../infra/moer/simplecharcontainer.hh \
	../infra/moer/simpledictionary.hh \
	../memory/segment.hh \

OBJ = \
	relation.o \
	nsm_relation.o \
	pax_relation.o \

all : $(OBJ)

relation.o : relation.cc relation.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) relation.cc

nsm_relation.o : nsm_relation.cc nsm_relation.hh relation.hh ../nsm/page_interpreter_sp.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) nsm_relation.cc

pax_relation.o : pax_relation.cc pax_relation.hh relation.hh ../pax/page_interpreter_pax.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) pax_relation.cc

clean :
	rm -f *.o a.out


