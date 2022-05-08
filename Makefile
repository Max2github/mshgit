# $@ ... target
# $? ... all dependencies - not sure
# $< ... first dependency
# $^ ... all dependencies - no duplicates
# $+ ... all dependencies - including duplicates

CC = gcc # compiler
CFLAGS = -c -Wall #-std=c++20 # flags
# executable / target file
EXE = msh


SRC_DIR := src
DEP_DIR := dependencies
OBJ_DIR := o
INC_DIR := include

#paths to other src directories
COM_SRC_DIR := $(SRC_DIR)/Commands
COM_MAIN_SRC_DIR := $(COM_SRC_DIR)/main
COM_SUB_SRC_DIR := $(COM_SRC_DIR)/sub
COM_DEP_SRC_DIR := $(COM_SRC_DIR)/dependencies

# source files in different folders
SRC_NORM := $(wildcard $(SRC_DIR)/*.c)
# as there are files with the same name, they will maintain the folder structure
SRC_COM := $(wildcard $(COM_SRC_DIR)/*.c) \
		$(wildcard $(COM_MAIN_SRC_DIR)/*.c) \
		$(wildcard $(COM_SUB_SRC_DIR)/*.c) \
		$(wildcard $(COM_DEP_SRC_DIR)/*.c)
# all source files + main.c
SRC := $(SRC_NORM) $(SRC_COM)
# all object files
OBJ := $(SRC_NORM:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) \
		$(SRC_COM:$(COM_SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# all source files of the dependencies / own libraries
DEP := $(wildcard $(DEP_DIR)/*.c)
# all .o files of the dependencies / own libraries
DEP_OBJ := $(DEP:$(DEP_DIR)/%.c=$(OBJ_DIR)/lib/%.o)

.PHONY: all clean objlib

all: $(EXE)
	./$<

objlib: $(OBJ) $(DEP_OBJ)
	ld -r -o $(EXE).o $^

$(EXE): $(OBJ) $(DEP_OBJ)
	$(CC) -o $@ $+

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(COM_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(COM_MAIN_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(COM_SUB_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(COM_DEP_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/lib/%.o: $(DEP_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f .DS_Store
	rm $(OBJ)
	rm $(DEP_OBJ)

-include $(OBJ:.o=.d)