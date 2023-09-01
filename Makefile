# NFD just add -I ../include/nfd_src -lnfd -lole32 -luuid
# nfd_src contains the .h files and remember to add nfd.lib into lib folder, contains implementations for nfd.h files

# Get OS
PLATFORM_OS ?= WINDOWS

# No uname.exe on MinGW!, but OS=Windows_NT on Windows!
# ifeq ($(UNAME),Msys) -> Windows
ifeq ($(OS),Windows_NT)
    PLATFORM_OS = WINDOWS
else
    UNAMEOS = $(shell uname -s)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS = LINUX
    endif
    ifeq ($(UNAMEOS),FreeBSD)
        PLATFORM_OS = BSD
    endif
    ifeq ($(UNAMEOS),OpenBSD)
        PLATFORM_OS = BSD
    endif
    ifeq ($(UNAMEOS),NetBSD)
        PLATFORM_OS = BSD
    endif
    ifeq ($(UNAMEOS),DragonFly)
        PLATFORM_OS = BSD
    endif
    ifeq ($(UNAMEOS),Darwin)
        PLATFORM_OS = OSX
    endif
endif

SRCEXTS := cpp c
HDREXTS := hpp h

MKDIR    := mkdir
REMOVE   := - rm
CC       := g++
BIN      := ./build
OBJ      := ./build/obj
INCLUDE  := ./src ./include ./src/ui ./export/assets
SRC      := ./src
SRCS     := $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/ui/*.cpp)
OBJS     := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))

ifeq ($(PLATFORM_OS),WINDOWS)
EXE      := CType.exe
else
EXE      := CType
endif

WARNINGS := -Wno-missing-braces -Wno-narrowing -Wno-sign-compare -Wno-char-subscripts
CFLAGS   := -O2 -Wall -std=c++20 $(WARNINGS)
LDFLAGS  := -std=c++20 -L ./lib 

ifeq ($(PLATFORM_OS),WINDOWS)
LDLIBS   := -licon -lraylib -lopengl32 -lgdi32 -lwinmm
endif
ifeq ($(PLATFORM_OS),LINUX)
LDLIBS   := ./lib/librayliblinux.a -lc -lm -lpthread -ldl -lrt -lX11
endif
 
IFLAGS   := $(foreach dir,$(INCLUDE),-I $(dir)/)

.PHONY: all run clean debug release default info export 


all: $(EXE)

default: all run

debug: CFLAGS+=-g3
debug: all

ifeq ($(PLATFORM_OS),WINDOWS)
release: LDLIBS+=-static
endif
release: LDLIBS+=-static-libgcc -static-libstdc++ 
ifeq ($(PLATFORM_OS),WINDOWS) # how to remove console for linux?
release: LDLIBS+=-Wl,--subsystem,windows
endif
release: all run

# quick TODO: add export linux support
export:
	g++ ./export/exporter.cpp -o ./export/exporter.exe -O2 -Wall -Wno-sign-compare -std=c++20 -I ./include -I ./export/assets -L ./lib -lraylib -lopengl32 -lgdi32 -lwinmm && ./export/exporter.exe 

relink: $(CC) $(IFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(EXE): $(OBJS) 
	@echo LINKING EXE:
	$(CC) $(IFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

vpath %.cpp $(SRC)

$(OBJ)/%.o: %.cpp 
	@echo COMPILING: $<
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

vpath %.cpp $(SRC)/ui

$(OBJ)/ui/%.o: %.cpp 
	@echo COMPILING: $<
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@


run: $(EXE)
	./$(EXE)

clean:
	$(REMOVE) $(OBJ)/*.o
	$(REMOVE) $(OBJ)/ui/*.o
	$(REMOVE) $(EXE)

info:
	@echo Objects: $(OBJS)
	@echo SourceDirs: $(SRCS)
	@echo IncludeDirs: $(IFLAGS)	
	@echo DEBUG: $(OBJ)/%.o