OBJDIR := obj
SRCDIR := .
RCDIR := .

TARGET := Clover.exe
CXX := g++
CXXFLAGS := -std=c++23 -municode
LDFLAGS :=  -mwindows
RC := windres
LIBS := -lcomctl32 -ld2d1

CPP_SOURCES := $(wildcard $(SRCDIR)/*.cpp)
RC_SOURCES := $(wildcard $(RCDIR)/*.rc)

CPP_OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES))
RC_OBJS := $(patsubst $(SRCDIR)/%.rc,$(OBJDIR)/%.o,$(RC_SOURCES))
ALL_OBJS := $(CPP_OBJS) $(RC_OBJS)

BUILD ?= release

ifeq ($(BUILD),debug)
	CXXFLAGS += -g
	LDFLAGS += -g
else
	CXXFLAGS +=  -O3
	LDFLAGS +=  -O3
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

