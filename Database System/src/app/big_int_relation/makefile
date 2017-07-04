#CFLAGS = -g -Wall -Weffc++ -std=c++0x
CFLAGS = -O3 -std=c++0x

CC = g++
# CC = g++-4.6

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	big_int_relation_data.hh \
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
	big_int_relation.o \

all : $(OBJ)

big_int_relation.o : big_int_relation.cc big_int_relation.hh $(HDR)
	$(CC) -c $(CFLAGS) $(INCL) big_int_relation.cc

clean :
	rm -f *.o a.out


