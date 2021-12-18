main:main.o Terminal.o doEcho.o fileProcess.o stringProcess.o doGrep.o doDiff.o
	g++ main.o Terminal.o doEcho.o fileProcess.o stringProcess.o doGrep.o doDiff.o -o main
Terminal.o:Terminal.h Terminal.cpp doEcho.h doGrep.h doDiff.h
	g++ -c Terminal.cpp -o Terminal.o
main.o:main.cpp Terminal.h
	g++ -c main.cpp -o main.o
doEcho.o:doEcho.h doEcho.cpp Terminal.h
	g++ -c doEcho.cpp -o doEcho.o
fileProcess.o:fileProcess.h fileProcess.cpp stringProcess.h Terminal.h
	g++ -c fileProcess.cpp -o fileProcess.o
stringProcess.o:stringProcess.h stringProcess.cpp
	g++ -c stringProcess.cpp -o stringProcess.o
doGrep.o:doGrep.h doGrep.cpp fileProcess.h Terminal.h
	g++ -c doGrep.cpp -o doGrep.o
doDiff.o:doDiff.h doDiff.cpp Terminal.h
	g++ -c doDiff.cpp -o doDiff.o
clean:
	rm *.o