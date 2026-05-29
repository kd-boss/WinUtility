.PHONY: all
CXX = g++ -std=c++23
LFLAGS = -mwindows -municode -lcomctl32 -ld2d1 -lOle32 -ldwrite
CFLAGSRELEASE = -s -O3
CFLAGSDEBUG = -g


debug:
		$(CXX) GdiInterop.cpp Main.cpp $(CFLAGSDEBUG) $(LFLAGS) -o GDiInterop.exe

all:
		$(CXX) GdiInterop.cpp Main.cpp $(CFLAGSRELEASE) $(LFLAGS) -o GdiInterop.exe

.PHONY: clean
clean:
	rm -rf GDiInterop.exe