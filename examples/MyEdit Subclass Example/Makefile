CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../../include
LDFLAGS = -mwindows -municode
LDLIBS = -luser32 -lgdi32 -lcomctl32

TARGET = ContainedEditTest.exe
OBJECTS = ContainedEditTest.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

ContainedEditTest.o: ContainedEditTest.cpp ContainedEditTest.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
