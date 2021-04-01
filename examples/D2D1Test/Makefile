.PHONY: all

CFLAGS = -mwindows -municode -s -O3

D2D1Test.exe:
	g++ D2D1Test.cpp $(CFLAGS) -ld2d1 -o D2D1Test.exe

.PHONY: clean
clean: 
	-rm *.exe

