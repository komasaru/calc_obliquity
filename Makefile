gcc_options = -std=c++17 -Wall -O2 --pedantic-errors

calc_obliquity: calc_obliquity.o obliquity.o time.o common.o
	g++92 $(gcc_options) -o $@ $^

calc_obliquity.o : calc_obliquity.cpp
	g++92 $(gcc_options) -c $<

obliquity.o : obliquity.cpp
	g++92 $(gcc_options) -c $<

time.o : time.cpp
	g++92 $(gcc_options) -c $<

common.o : common.cpp
	g++92 $(gcc_options) -c $<

run : calc_obliquity
	./calc_obliquity

clean :
	rm -f ./calc_obliquity
	rm -f ./*.o

.PHONY : run clean

