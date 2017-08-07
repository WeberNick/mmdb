#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -Wall -Weffc++ -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	relation_infra.hh \
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
	$(CC) -c $(CXXFLAGS) $(INCL) relation.cc

nsm_relation.o : nsm_relation.cc nsm_relation.hh relation.hh
	$(CC) -c $(CXXFLAGS) $(INCL) nsm_relation.cc

pax_relation.o : pax_relation.cc pax_relation.hh relation.hh
	$(CC) -c $(CXXFLAGS) $(INCL) pax_relation.cc

clean :
	rm -f *.o a.out


