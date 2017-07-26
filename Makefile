.PHONY : all clean
CXX := g++
CXXFLAGS := -std=c++11 -O3 -D NDEBUG
BINDIR := bin
SRCDIR := .
TARGET := $(BINDIR)/io_test
SOURCE := $(SRCDIR)/io_test.cpp
OUTPUT := test_file.tmp

all : $(TARGET)

$(TARGET) : $(SOURCE) | $(BINDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BINDIR) :
	mkdir $@

clean :
	$(RM) $(TARGET) $(OUTPUT)
