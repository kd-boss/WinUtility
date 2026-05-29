RC = windres
BUILD = ./obj

.PHONY: all

	
objects = $(BUILD)/COLORS1Res.o
CFLAGS = -mwindows -municode -mwin32 -O2 -lcomctl32
CXX = g++ -std=c++23

wintest.exe : $(objects)
	$(CXX) COLORS1.cpp $(objects) $(CFLAGS) -o COLORS1.exe 

$(objects): COLORS1.rc | $(if $(BUILD), mkdir)
	$(RC) -i COLORS1.rc -o $(BUILD)/COLORS1Res.o

mkdir:
	mkdir $(BUILD)
	
.PHONY : clean
clean:
	rm -rf $(BUILD)
	rm COLORS1.exe 
	
debug : $(objects)
	$(CXX) COLORS1.cpp $(objects) -mwindows -municode -fconcepts-diagnostics-depth=2 -g -lcomctl32 -o COLORS1.exe



