.PHONY : all clean
CXX := g++
CXXFLAGS := -std=c++11 -O3 -D NDEBUG
BINDIR := bin
SRCDIR := .
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
TARGETS := $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%)
OUTPUT := test_file.tmp

all : $(TARGETS)

$(BINDIR)/% : $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BINDIR) :
	mkdir $@

clean :
	$(RM) $(TARGETS) $(OUTPUT)
