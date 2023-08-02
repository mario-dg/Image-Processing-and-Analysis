CC = g++
CPPFLAGS += -g -ggdb # Debugsymbole haben
CPPFLAGS += -Werror -Wall -Wextra # alle Warnungen aktivieren
CPPFLAGS += --std=c++14 # C++17 braucht OpenCV neuer als 3.2.0
#CPPFLAGS += -Wconversion -Wsign-conversion # zu nervig 
CPPFLAGS += -Wsign-compare 
CPPFLAGS += -Wvla # C-style variable length arrays verbieten
SRCS = $(shell find . -type f -name '*.cpp')
HEDS = $(shell find . -type f -name '*.hpp')
OBJS = $(SRCS:%.cpp=%.o)
PROG = bba

CPPFLAGS += `pkg-config opencv --cflags`
LIBS += `pkg-config opencv --libs`

all: $(PROG)

.PHONY: clean all

$(PROG): .depend $(OBJS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(OBJS) $(LIBS)

clean: 
	rm -f $(PROG)
	rm -f $(OBJS)
	rm -f .depend

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

# Compute header file dependencies 
.depend : $(SRCS)
	$(CC) $(CPPFLAGS) -MM $^ > .depend

# Respect header file dependencies
include .depend
