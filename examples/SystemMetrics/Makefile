RC = windres
BUILD = ./obj

.PHONY: all

	
objects = $(BUILD)/SystemMetricsRes.o
CFLAGS = -mwindows -municode -s -O2 -lcomctl32
CXX = g++ -std=c++23

SystemMetrics.exe : $(objects)
	$(CXX) SystemMetrics.cpp $(objects) $(CFLAGS) -o SystemMetrics.exe 

$(objects): SystemMetrics.rc | $(if $(BUILD), mkdir)
	$(RC) -i SystemMetrics.rc -o $(BUILD)/SystemMetricsRes.o

mkdir:
	mkdir $(BUILD)
	
.PHONY : clean
clean:
	rm -rf $(BUILD)
	rm SystemMetrics.exe 
	
debug : $(objects)
	$(CXX) SystemMetrics.cpp $(objects) -mwindows -municode -g -lcomctl32 -o SystemMetrics.exe


