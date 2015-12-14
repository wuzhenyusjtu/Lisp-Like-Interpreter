#!bin/bash

# Name of the executable; should match the contents of Runfile
TARGET = Interpreter

# C++ compiler
CXX = g++

DEBUG = -g

# C++ compiler flags
CXXFLAGS = -Wall -c $(DEBUG)
LXXFLAGS = -Wall $(DEBUG)
C++11 = -std=c++0x
# Creating a .o file
COMPILE = $(CXX) $(CXXFLAGS) $(C++11)
LINK = $(CXX) $(LXXFLAGS) -o

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES:= $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINK) $@ $(OBJECTS)
	@echo "Linking complete!"
	
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(COMPILE) $< -o $@
	@echo "Compiled "$<" successfully!"

$(shell ln -s $(BINDIR)/$(TARGET) Interpreter)

clean:
	\rm $(OBJDIR)/* $(BINDIR)/* Interpreter
