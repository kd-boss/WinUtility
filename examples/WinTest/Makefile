RC = windres
BUILD = ./obj

.PHONY: all

	
objects = $(BUILD)/wintestres.o
CFLAGS = -fno-rtti -mwindows -municode -s -O2 -lcomctl32
CXX = g++

wintest.exe : $(objects)
	$(CXX) wintest.cpp $(objects) $(CFLAGS) -o wintest.exe 

$(objects): wintestres.rc | $(if $(BUILD), mkdir)
	$(RC) -i wintestres.rc -o $(BUILD)/wintestres.o

mkdir:
	mkdir $(BUILD)
	
.PHONY : clean
clean:
	-rm wintest.exe $(objects)
	-rm -r -f $(BUILD)



