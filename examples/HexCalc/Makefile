OBJDIR := obj
SRCDIR := .
RCDIR := .

TARGET := hexcalc.exe
CXX := g++
CXXFLAGS := -std=c++23 -municode -DUNICODE -D_UNICODE -Wall -Werror -Wextra -Wpedantic -Wshadow
CXXFLAGS += -Wformat=2 -Wconversion -Wduplicated-cond -Wduplicated-branches -Wignored-qualifiers
CXXFLAGS += -Wsign-conversion -Wcast-qual -Wold-style-cast -Woverloaded-virtual 
CXXFLAGS += -Wnon-virtual-dtor -Wsuggest-override -Wlogical-op -Wnull-dereference
CXXFLAGS += -Wunused -Wreturn-type -Wmissing-field-initializers 
LDFLAGS := -mwindows -municode -mconsole
RC := windres
LIBS := -lcomctl32 -lstdc++exp

CPP_SOURCES := $(wildcard $(SRCDIR)/*.cpp)
RC_SOURCES := $(wildcard $(RCDIR)/*.rc)

CPP_OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES))
RC_OBJS := $(patsubst $(SRCDIR)/%.rc,$(OBJDIR)/%.o,$(RC_SOURCES))
ALL_OBJS := $(CPP_OBJS) $(RC_OBJS)

BUILD ?= release

ifeq ($(BUILD),debug)
	CXXFLAGS += -g3 -Wall -Wpedantic
	LDFLAGS += -g3
else
	CXXFLAGS += -O3 -s
	LDFLAGS += -O3 -s
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

