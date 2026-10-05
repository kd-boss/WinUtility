CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -municode -DUNICODE -D_UNICODE 
LDFLAGS = -mwindows -municode
LDLIBS = -luser32 -lgdi32 -lcomctl32 -lstdc++exp

TARGET = ContainedEditTest.exe
OBJECTS = ContainedEditTest.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

ContainedEditTest.o: ContainedEditTest.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
