
CC            = gcc
CXX           = g++ #-m64
CFLAGS        = -g -Wall

DEL_FILE      = rm
CREATE_DIR    = mkdir -p

SOURCE_DIR    = src
OBJECT_DIR    = obj
SOURCES       = $(SOURCE_DIR)/main.cpp \
    $(SOURCE_DIR)/nodes.cpp \
    $(SOURCE_DIR)/synapses.cpp \
    $(SOURCE_DIR)/core.cpp 
OBJECTS       = $(OBJECT_DIR)/main.o \
    $(OBJECT_DIR)/nodes.o \
    $(OBJECT_DIR)/synapses.o \
    $(OBJECT_DIR)/core.o 
DESTDIR_TARGET = hem.exe

first: all

all: Makefile $(DESTDIR_TARGET)
	@echo ' '
	@echo 'Program succesfully built!'

$(DESTDIR_TARGET): dir $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(DESTDIR_TARGET) $(OBJECTS)

dir: 
	$(CREATE_DIR) obj
	$(CREATE_DIR) data

clean:
	-$(DEL_FILE) $(OBJECTS)

reallyclean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) data/*

$(OBJECT_DIR)/main.o: $(SOURCE_DIR)/main.cpp \
    $(SOURCE_DIR)/core.h \
    $(SOURCE_DIR)/nodes.h \
    $(SOURCE_DIR)/synapses.h
	$(CXX) -c $(CFLAGS) -o $(OBJECT_DIR)/main.o $(SOURCE_DIR)/main.cpp

$(OBJECT_DIR)/core.o: $(SOURCE_DIR)/core.cpp \
    $(SOURCE_DIR)/core.h \
    $(SOURCE_DIR)/nodes.h \
    $(SOURCE_DIR)/synapses.h
	$(CXX) -c $(CFLAGS) -o $(OBJECT_DIR)/core.o $(SOURCE_DIR)/core.cpp

$(OBJECT_DIR)/nodes.o: $(SOURCE_DIR)/nodes.cpp \
    $(SOURCE_DIR)/nodes.h 
	$(CXX) -c $(CFLAGS) -o $(OBJECT_DIR)/nodes.o $(SOURCE_DIR)/nodes.cpp

$(OBJECT_DIR)/synapses.o: $(SOURCE_DIR)/synapses.cpp \
    $(SOURCE_DIR)/synapses.h 
	$(CXX) -c $(CFLAGS) -o $(OBJECT_DIR)/synapses.o $(SOURCE_DIR)/synapses.cpp
