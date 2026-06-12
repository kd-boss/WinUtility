OBJDIR := obj
SRCDIR := .
RCDIR := .
GXX := g++
IS_GXX := $(shell $(GXX) -v 2>&1 | grep -q "gcc version" && echo yes || echo no)

TARGET := Com0Com.exe

ifeq ($(IS_GXX),yes)
CXX := g++
else
CXX := Clang++
endif

ifeq ($(IS_GXX),yes)
CXXFLAGS := -std=c++23 -mthreads -mwin32 -municode -D_UNICODE
LDFLAGS := -mwindows -static-libgcc -static-libstdc++
else
CXXFLAGS := -std=c++23 -municode -D_UNICODE
LDFLAGS := -mwindows
endif


RC := windres
LIBS := -lpropsys -lsetupapi -lcomctl32 -lshlwapi -ldwmapi -luxtheme -luser32 -lgdi32 -ladvapi32 -lshell32 -luuid

CPP_SOURCES := $(wildcard $(SRCDIR)/*.cpp)
RC_SOURCES := $(wildcard $(RCDIR)/*.rc)

CPP_OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES))
RC_OBJS := $(patsubst $(SRCDIR)/%.rc,$(OBJDIR)/%.o,$(RC_SOURCES))
ALL_OBJS := $(CPP_OBJS) $(RC_OBJS)

BUILD ?= release


ifeq ($(BUILD),debug)
	CXXFLAGS += -g -Wall -Wpedantic -DDEBUG 
	LDFLAGS += -g  
else
	CXXFLAGS += -O3 -s
	LDFLAGS += -O3
endif

.PHONY: all clean debug

all: $(TARGET) 

$(OBJDIR):
		mkdir -p $(OBJDIR)

$(TARGET): $(ALL_OBJS) $(OBJDIR)
		$(CXX) $(CXXFLAGS) $(ALL_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
		$(CXX) $(CXXFLAGS) -c  $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.rc | $(OBJDIR)
		$(RC) $< -O coff -o $@

clean:
		rm -rf $(OBJDIR) $(TARGET)

debug:
	@$(MAKE) all BUILD=debug

