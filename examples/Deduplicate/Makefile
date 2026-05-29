
CXX = g++ 
CFLAGS = -std=c++23 -s -O3 -DUNICODE
LFLAGS = -lCrypt32 -lbcrypt -o

.PHONY : all

release :
	$(CXX) $(CFLAGS) deduplicate.cpp $(LFLAGS) deduplicate.exe 

.PHONY : clean
clean:
	rm deduplicate.exe

debug:
	$(CXX) -std=c++23 -g deduplicate.cpp $(LFLAGS) deduplicate.exe
