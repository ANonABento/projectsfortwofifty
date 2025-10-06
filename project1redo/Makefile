all: a.out

a.out: dq.o core.o cpu.o main.o
	g++ -std=c++17 dq.o core.o cpu.o main.o -o a.out

dq.o: dq.cpp dq.h
	g++ -std=c++17 -c dq.cpp

core.o: core.cpp core.h dq.h
	g++ -std=c++17 -c core.cpp

cpu.o: cpu.cpp cpu.h core.h
	g++ -std=c++17 -c cpu.cpp

main.o: main.cpp cpu.h
	g++ -std=c++17 -c main.cpp

clean:
	rm -f *.o a.out
