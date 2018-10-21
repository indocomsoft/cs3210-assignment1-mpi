appname := sim

defs := -DDEBUG

CC := mpicc
CFLAGS := -g -Wall ${defs}

CXX := mpicc
# LDFLAGS :=  -fopenmp -pthread -lm

srcfiles := $(shell find . -name "*.c")
objects  := $(patsubst %.c, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(LDFLAGS) -o $(appname) $(objects)

clean:
	rm -f $(objects)
	rm -f $(appname)

dist-clean: clean
	rm -f *~ .depend
