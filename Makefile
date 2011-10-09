cxxobjects := main.o
cobjects := serial.o
objects := $(cobjects) $(cxxobjects)
version := 2.0

CC := gcc
CXX := g++
PLATFORM := $(shell uname -s)

EXTRA_CXXFLAGS := $(shell pkg-config --cflags gtkmm-3.0)
EXTRA_LIBS := $(shell pkg-config --libs gtkmm-3.0) -I/usr/include/linux/

ifdef DEBUG
CFLAGS += -ggdb
else
CFLAGS += -O2
endif
 
CXXFLAGS += $(EXTRA_CXXFLAGS) -Wall -Wextra -ansi
CFLAGS += -g -c -Wall -Wstrict-prototypes -Wextra -ansi -std=c99 -DOSNAME_LINUX
LDFLAGS := -Wl,--no-undefined
 
cxxsources := $(objects.o=.cc)
csources := $(objects.o =.c)
deps := $(objects:.o=.d)

.PHONY: clean

# from Lauri Leukkunen's build system
ifdef V
Q =
P = @printf "" # <- space before hash is important!!!
else
P = @printf "[%s] $@\n" # <- space before hash is important!!!
Q = @
endif

#ejecutable.. aqui
main := main.bin

all: $(main)
$(main): $(cxxobjects) $(cobjects)
$(main): CFLAGS := $(CFLAGS) $(EXTRA_CXXFLAGS) -D VERSION='"$(version)"'
$(main): LIBS := $(EXTRA_LIBS)

$(main):
	$(P)LINK
	$(Q)$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
%.o:: %.c
	$(P)CC
	$(Q)$(CC) $(CFLAGS) -MMD -o $@ -c $<
 
%.o:: %.cc
	$(P)CXX
	$(Q)$(CXX) $(CXXFLAGS) -MMD -o $@ -c $<

clean:
	  rm -f $(main) $(objects) $(deps)
