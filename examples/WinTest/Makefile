RC = windres
BUILD = ./obj

.PHONY: all

	
objects = $(BUILD)/wintestres.o
CFLAGS = -mwindows -municode -s -O3 -lcomctl32

wintest.exe : $(objects)
	g++ wintest.cpp $(objects) $(CFLAGS) -o wintest.exe 

$(objects): wintestres.rc | $(if $(BUILD), mkdir)
	$(RC) -i wintestres.rc -o $(BUILD)/wintestres.o

mkdir:
	mkdir $(BUILD)
	
.PHONY : clean
clean:
	-rm wintest.exe $(objects)
	-rm -r -f $(BUILD)



