#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	tpc_h_data.hh \
	../../infra/moer/measure.hh \
	../../infra/webe/common.hh \
	../../infra/webe/exception.hh \
	../../infra/webe/print_helper.hh \
	../../infra/webe/types.hh \
	../../infra/webe/bulk_loader.hh \
	../../nsm/bulk_insert_sp.hh \
	../../pax/bulk_insert_pax.hh \
	../../memory/memory_manager.hh \
	../../relation/relation.hh \
	../../relation/nsm_relation.hh \
	../../relation/pax_relation.hh \

OBJ = \
	tpc_h.o \

all : $(OBJ)

tpc_h.o : tpc_h.cc tpc_h.hh $(HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) tpc_h.cc

clean :
	rm -f *.o a.out


