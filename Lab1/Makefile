#
#  To sa opcje dla kompilacji
#
CXXFLAGS=-g -Iinc -Wall -pedantic -std=c++0x


run: obj obj/permutation.o obj/cmax.o obj/main.o
	g++ ${CXXFLAGS} -o run obj/permutation.o obj/cmax.o obj/main.o 

obj:
	mkdir obj

obj/permutation.o: src/permutation.cpp inc/permutation.hh  inc/cmax.hh
	g++ -c ${CXXFLAGS} -o obj/permutation.o src/permutation.cpp

obj/cmax.o: src/cmax.cpp inc/cmax.hh 
	g++ -c ${CXXFLAGS} -o obj/cmax.o src/cmax.cpp

obj/main.o: src/main.cpp inc/cmax.hh inc/permutation.hh
	g++ -c ${CXXFLAGS} -o obj/main.o src/main.cpp

clean:
	rm -f obj/*.o run
	rm src/*.*~
	rm inc/*.*~
	rm *~
