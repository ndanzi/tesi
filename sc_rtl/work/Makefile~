.SUFFIXES: .cc .cpp .o .x

CC     = g++
CFLAGS = -c -Wall -std=c++11 -Wno-parentheses -ggdb #-O3

SRCS = ../src/m6502.cc \
	   ../src/tb.cc \
	   ../src/main.cc

OBJS = $(SRCS:../src/%.cc=%.o)

SYSTEMC_HOME = /home/nicolo/hifsuite/third_party/sandbox/systemc/Linux/x86_64
HIF_HOME = /home/nicolo/hifsuite/core/sandbox/obj_linux_64/hifsuite-stable-linux-x86_64
HDTLIB_HOME = /home/nicolo/hifsuite/hdtlib/sandbox/obj_linux_64/hifsuite-stable-linux-x86_64

INCDIR = -I$(SYSTEMC_HOME)/include \
		 -I$(HIF_HOME)/include \
		 -I$(HDTLIB_HOME)/include

all: sim.x

sim.x: $(OBJS)
	$(CC) -o $@ $(OBJS) $(SYSTEMC_HOME)/lib/libsystemc.a $(HIF_HOME)/lib/libhif2scSupport.a $(HDTLIB_HOME)/lib/libhdtlib.a -lpthread 2>&1 | c++filt
	rm -f $(OBJS)

%.o: ../src/%.cc
	$(CC) $(CFLAGS) $(INCDIR) $< -o $@

clean:
	rm -f $(OBJS) sim.x

.PHONY: all clean
