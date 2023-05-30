#default:
#	g++ ../main.cpp -o main.exe -O2 -Wall -Wno-missing-braces -Wno-narrowing -std=c++20 -I ../ -I ../include/ -L ../lib/ -I ../include/nfd_src -lnfd -lole32 -luuid -lraylib -lopengl32 -lgdi32 -lwinmm && ../build/main.exe

#debug:
#	g++ -g3 ../main.cpp -o main.exe -O2 -Wall -Wno-missing-braces -Wno-narrowing -std=c++20 -I ../ -I ../include/ -L ../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

#final: 
#	g++ ../main.cpp -o main.exe -Ofast -Wall -Wno-missing-braces -Wno-narrowing -std=c++20 -I ../ -I ../include/ -L ../lib/ -I ../include/nfd_src -lnfd -lole32 -luuid -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++ -Wl,--subsystem,windows && ../build/main.exe

# NFD just add -I ../include/nfd_src -lnfd -lole32 -luuid
# nfd_src contains the .h files and remember to add nfd.lib into lib folder, contains implementations for nfd.h files
SRCEXTS := cpp c
HDREXTS := hpp h

MKDIR    := mkdir
REMOVE   := - rm
CC       := g++
BIN      := ./build
OBJ      := ./build/obj
INCLUDE  := ./src ./include ./include/nfd_src ./src/ui
SRC      := ./src
SRCS     := $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/ui/*.cpp)
OBJS     := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
EXE      := $(BIN)/main.exe
WARNINGS := -Wno-missing-braces -Wno-narrowing -Wno-sign-compare -Wno-char-subscripts
CFLAGS   := -O2 -Wall -std=c++20 $(WARNINGS)
LDFLAGS  := -std=c++20 -L ./lib 
LDLIBS   := -lnfd -lole32 -luuid -lraylib -lopengl32 -lgdi32 -lwinmm
IFLAGS   := $(foreach dir,$(INCLUDE),-I $(dir)/)

.PHONY: all run clean debug default info 


all: $(EXE)

default: all run

debug: CFLAGS+=-g3

debug: clean all

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
	$<

clean:
	$(REMOVE) $(OBJ)/*.o
	$(REMOVE) $(OBJ)/ui/*.o
	$(REMOVE) $(EXE)

info:
	@echo Objects: $(OBJS)
	@echo SourceDirs: $(SRCS)
	@echo IncludeDirs: $(IFLAGS)	
	@echo DEBUG: $(OBJ)/%.o