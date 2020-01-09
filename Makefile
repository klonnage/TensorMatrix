CPP_FLAGS=-O3 -march=native -mtune=native -std=c++11 -fopenmp -mavx

all: create-ttmat create-ttvec compare-ttvec ttmatvec

ttmatvec: ttmatvec.cpp ttmat.cpp ttmat.h ttvec.cpp ttvec.h
	g++ ttmatvec.cpp ttmat.cpp ttvec.cpp -lm -o ttmatvec	

create-ttmat: create-ttmat.cpp
	g++ ${CPP_FLAGS} create-ttmat.cpp -o create-ttmat

create-ttvec: create-ttvec.cpp
	g++ ${CPP_FLAGS} create-ttvec.cpp -o create-ttvec

compare-ttvec: compare-ttvec.cpp ttvec.cpp
	g++ ${CPP_FLAGS} compare-ttvec.cpp ttvec.cpp -lm -o compare-ttvec

clean:
	rm -f ttmatvec create-ttmat create-ttvec compare-ttvec
