#CXXFLAGS = -g -Wall -Weffc++ -std=c++0x
CXXFLAGS = -O2 -g -Wall -Weffc++ -std=c++0x
#CXXFLAGS = -O3 -Wall -Weffc++ -std=c++0x

CC = g++

INCL = -I../.. -I..

VPATH = .:..:../..

HDRQ = \
	../infra/moer/measure.hh \
	../infra/webe/print_helper.hh \
	../infra/webe/types.hh \
	../infra/webe/common.hh \
	\
	../relation/nsm_relation.hh \
	../relation/pax_relation.hh \
	\
	../ops/top.hh \
	../ops/project.hh \
	../ops/project_mat.hh \
	../ops/select.hh \
	../ops/scan.hh \
	\


OBJ = \
	test_query.o \
	test_pred.o \
	
all : $(OBJ)

test_query.o : test_query.cc test_query.hh $(HDRQ) test_pred.hh
	$(CC) -c $(CXXFLAGS) $(INCL) test_query.cc

test_pred.o : test_pred.cc test_pred.hh $(HDRQ) ../pax/page_interpreter_pax.hh
	$(CC) -c $(CXXFLAGS) $(INCL) test_pred.cc

clean :
	rm -f *.o a.out


