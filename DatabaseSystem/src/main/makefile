#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -std=c++0x
#CXXFLAGS = -O3 -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

HDR = \
	../memory/memory_manager.hh \
	../memory/segment.hh \
	\
	../infra/webe/types.hh \
	../infra/webe/common.hh \
	../infra/webe/bulk_loader.hh \
	../infra/webe/print_helper.hh \
	../infra/webe/args.hh \
	\
	../infra/moer/system.hh \
	../infra/moer/memory_access.hh \
	\
	../relation/relation.hh \
	../relation/nsm_relation.hh \
	../relation/pax_relation.hh \
	\
	../nsm/page_interpreter_sp.hh \
	../nsm/bulk_insert_sp.hh \
	\
	../pax/page_interpreter_pax.hh \
	../pax/bulk_insert_pax.hh \
	\
	../app/tpc_h/tpc_h_data.hh \
	../app/tpc_h/tpc_h.hh \
	\
	../app/big_int_relation/big_int_relation_data.hh \
	../app/big_int_relation/big_int_relation.hh \
	\
	../ops/scan.hh \
	../ops/select.hh \
	../ops/project.hh \
	../ops/project_mat.hh \
	../ops/project_optimized_switch.hh \
	../ops/top.hh \
	\
	\
	../queries/test_query.hh \
	../queries/test_update.hh \
	\
	modes.hh \


OBJ = \
	../memory/memory_manager.o \
	../memory/segment.o \
	\
	../infra/webe/exception.o \
	../infra/webe/bulk_loader.o \
	../infra/webe/print_helper.o \
	../infra/webe/args.o \
	\
	../infra/moer/datejd.o \
	../infra/moer/linereader.o \
	../infra/moer/simplecharcontainer.o \
	../infra/moer/simpleexception.o \
	../infra/moer/stime.o \
	../infra/moer/stimestamp.o \
	../infra/moer/system.o \
	../infra/moer/memory_access.o \
	\
	../relation/relation.o \
	../relation/nsm_relation.o \
	../relation/pax_relation.o \
	\
	../nsm/page_interpreter_sp.o \
	../nsm/bulk_insert_sp.o \
	\
	../pax/page_interpreter_pax.o \
	../pax/bulk_insert_pax.o \
	\
	../app/tpc_h/tpc_h.o \
	\
	../app/big_int_relation/big_int_relation.o \
	\
	../queries/test_query.o \
	../queries/test_pred.o \
	\

ALL = main 


all : $(ALL)

main: main.o $(OBJ)
	$(CC) -o main main.o $(OBJ)

main.o : main.cc $(HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) main.cc

clean :
	rm -f *.o a.out $(ALL)


