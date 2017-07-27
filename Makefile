.PHONY : all clean test testsimple testp testdirect testmmap
CXXFLAGS := -std=c++11 -O3 -D NDEBUG
BINDIR := bin
SRCDIR := .
SOURCE := $(wildcard $(SRCDIR)/*.cpp)
TARGET := $(SOURCE:$(SRCDIR)/%.cpp=$(BINDIR)/%)
OUTPUT := test_file.tmp

all : $(TARGET)

$(BINDIR)/% : $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(BINDIR) :
	mkdir $@

testsimple :
	$(TARGET) simple 512  10000
	$(TARGET) simple 1024 10000
	$(TARGET) simple 2048 10000
	$(TARGET) simple 4096 10000
	$(TARGET) simple 8192 10000

testp :
	$(TARGET) p 512  10000
	$(TARGET) p 1024 10000
	$(TARGET) p 2048 10000
	$(TARGET) p 4096 10000
	$(TARGET) p 8192 10000

testdirect :
	$(TARGET) direct 512  10000
	$(TARGET) direct 1024 10000
	$(TARGET) direct 2048 10000
	$(TARGET) direct 4096 10000
	$(TARGET) direct 8192 10000

testmmap :
	$(TARGET) mmap 4096 10000
	$(TARGET) mmap 8192 10000

test : testsimple testp testdirect testmmap

clean :
	$(RM) -r $(BINDIR) $(OUTPUT)
