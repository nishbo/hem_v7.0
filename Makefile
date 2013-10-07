
CC            = gcc
CXX           = g++ #-m64
CFLAGS        = -g -Wall

DEL_FILE      = rm
CREATE_DIR    = mkdir -p

SOURCE_DIR    = src
OBJECT_DIR    = obj
SOURCES       = $(SOURCE_DIR)/main.cpp 
OBJECTS       = $(OBJECT_DIR)/main.o 
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
	-$(DEL_FILE) data/* export/* import/*

$(OBJECT_DIR)/main.o: $(SOURCE_DIR)/main.cpp 
	$(CXX) -c $(CFLAGS) -o $(OBJECT_DIR)/main.o $(SOURCE_DIR)/main.cpp
