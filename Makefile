CPP_FLAGS=-O3 -g -march=native -mtune=native -std=c++11 -fopenmp -mavx
CPP_FLAGS += -DOMP2
CPP_FLAGS += -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl
SRC_TTMAT = ttmat.cpp

all: create-ttmat create-ttvec compare-ttvec ttmatvec

ttmatvec: ttmatvec.cpp $(SRC_TTMAT) ttmat.h ttvec.cpp ttvec.h
	g++ ${CPP_FLAGS} ttmatvec.cpp $(SRC_TTMAT) ttvec.cpp -lm -o ttmatvec

create-ttmat: create-ttmat.cpp
	g++ ${CPP_FLAGS} create-ttmat.cpp -o create-ttmat

create-ttvec: create-ttvec.cpp
	g++ ${CPP_FLAGS} create-ttvec.cpp -o create-ttvec

compare-ttvec: compare-ttvec.cpp ttvec.cpp
	g++ ${CPP_FLAGS} compare-ttvec.cpp ttvec.cpp -lm -o compare-ttvec

clean:
	rm -f ttmatvec create-ttmat create-ttvec compare-ttvec

N = 1024
R = 8#64
NB_THREADS = 20

test_init: all
	./create-ttvec -f ttvec.bin -d 3 -m $(N),$(N),$(N) -r $(R),$(R)
	./create-ttmat -f ttmat.bin -d 3 -m $(N),$(N),$(N) -n $(N),$(N),$(N) -r $(R),$(R)


test: clean all
	export OMP_NUM_THREADS=$(NB_THREADS)
	./ttmatvec -a ttmat.bin -x ttvec.bin -y test.bin
	./compare-ttvec -x res.bin -y test.bin
