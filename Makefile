# $@ ... target
# $? ... all dependencies - not sure
# $< ... first dependency
# $^ ... all dependencies - no duplicates
# $+ ... all dependencies - including duplicates

CC = gcc # compiler
LD = ld # linker
CFLAGS = -c -Wall #-std=c++20 # flags for compiler
LD_FLAGS =  # flags for linker
# executable / target file
EXE_BASE_NAME := msh
EXE = $(EXE_BASE_NAME)


SRC_DIR := src
OBJ_DIR := o
INC_DIR := include

ifdef dep
	ifeq (dep, false)
# set to none - this may generate an error
DEP := 
DEP_DIR := none
DEP_OBJ := $(wildcard $(OBJ_DIR)/lib/*.o)
	endif
else
DEP_DIR := dependencies
# all source files of the dependencies / own libraries
DEP := $(wildcard $(DEP_DIR)/*.c)
# all .o files of the dependencies / own libraries
DEP_OBJ := $(DEP:$(DEP_DIR)/%.c=$(OBJ_DIR)/lib/%.o)
endif

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

# check for host / target
# windows_x86_32 | ✔
# windows_x86_64 | ✔
# macos_i386     | x
# linux_arm      | x
# macos_x86_64   | x
# linux_x86_64   | x
# linux_x86_32   | x

# x ... always (with clang it is always this way...) :
# src/../dependencies/std.h:1:10: fatal error: stdio.h: No such file or directory
#     1 | #include <stdio.h>
#       |          ^~~~~~~~~
# compilation terminated.
ifdef host
	ifneq (,$(filter $(host),macos_i386 macos_x86_32 macos_x86 macos_arm macos_arm64 macos_aarch64 macos_m1 macos_x86_64))
host = macos
	endif
else
host = macos
endif
ifdef target
# filter: if $(host) matches one of the names
# ifneq : is the result from filter not empty?
	ifneq (,$(filter $(target),macos_i386 macos_x86_32 macos_x86))
CC = gcc
LD = ld
CFLAGS += --target=i386-apple-darwin-macho
CLD_FLAGS += --target=i386-apple-darwin-macho
	endif
	ifneq (,$(filter $(target),linux_arm linux_arm64 linux_aarch64 linux_m1))
# macos: brew : aarch64-elf-gcc, aarch64-elf-binutils 
		ifeq ($(host), macos)
#CC = aarch64-none-elf-gcc
CC = aarch64-unknown-linux-gnu-gcc
LD = aarch64-unknown-linux-gnu-ld
#LD = aarch64-elf-gcc
#LD_FLAGS += -L /usr/local/lib/aarch64-elf/bfd-plugins #-ldep #/usr/local/lib/gcc/aarch64-elf/12.1.0
#LD = arm-linux-gnueabihf-ld
#LD = aarch64-none-elf-ld
#CC = arm-none-eabi-gcc
#LD = arm-none-eabi-ld
#CFLAGS += -march=armv8-a
#CFLAGS += --sysroot=/Applications/Arm/
#LD_FLAGS += --sysroot=/Applications/Arm/
#LD_FLAGS += --sysroot=/Applications/Arm/aarch64-none-elf/
#CFLAGS += -Os -flto -ffunction-sections -fdata-sections
#LD_FLAGS += -L /Applications/ARM/bin/../lib/gcc/arm-none-eabi/11.2.1/
#LD_FLAGS += -L /Applications/ARM/bin/../lib/gcc/aarch64-none-elf/11.2.1/ -lgcc -lcaf_single -lgcov $(wildcard /usr/local/lib/gcc/aarch64-elf/12.1.0/*.o)
#LD_FLAGS += -L. -L /Applications/Arm/aarch64-none-elf/lib -Map=$(EXE_BASE_NAME).map #-T /Applications/Arm/aarch64-none-elf/ldscripts/gcc.ld
#LD_FLAGS += --sysroot=/Applications/Arm/aarch64-none-elf/ -L /Applications/Arm/aarch64-none-elf/lib -lc --specs=/Applications/Arm/aarch64-none-elf/lib/rdimon.specs
#LD_FLAGS += -L /Applications/Arm/aarch64-unknown-linux-gnu/lib/gcc/aarch64-unknown-linux-gnu/8.3.0
#LD_FLAGS += --sysroot=/Applications/Arm/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -L /Applications/Arm/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot/lib -lgcc_s
#CC = clang
#LD = ld
#CFLAGS += --target=arm64-unknown-linux-elf
#LD_FLAGS += --target=arm64-unknown-linux-elf
		endif
	endif
	ifneq (,$(filter $(target),linux_i386 linux_x86_32 linux_x86))
# macos : brew : x86_64-elf-binutils, x86_64-elf-gcc, x86_64-elf-gdb
		ifeq ($(host), macos)
#export PATH="/usr/local/Cellar/x86_64-elf-binutils/2.38/bin/:/usr/local/Cellar/x86_64-elf-gcc/12.1.0/bin/:/usr/local/Cellar/i386-elf-gdb/12.1/bin:$PATH"
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
CFLAGS += -m32 #--sysroot=/usr/local/Cellar/x86_64-elf-gcc/12.1.0/
LD_FLAGS += -m elf_i386
		endif
	endif
	ifneq (,$(filter $(target),windows_i386 windows_x86_32 windows_x86))
# macos : macports : i686-w64-mingw32-gcc, x86_64-w64-mingw32-gcc
		ifeq ($(host), macos)
CC = i686-w64-mingw32-gcc
LD = i686-w64-mingw32-ld
		endif
	endif
	ifeq ($(target), windows_x86_64)
		ifeq ($(host), macos)
CC = x86_64-w64-mingw32-gcc
LD = x86_64-w64-mingw32-ld
		endif
	endif
	ifeq ($(target), macos_x86_64)
		ifeq ($(host), macos)
CC = gcc
LD = ld
CFLAGS += --target=x86_64-apple-darwin-macho
CLD_FLAGS += --target=x86_64-apple-darwin-macho
		endif
	endif
	ifeq ($(target), linux_x86_64)
		ifeq ($(host), macos)
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
CFLAGS += -m64 -I lib/gcc/x86_64-elf/12.1.0/include
LD_FLAGS += -m elf_x86_64 -L lib/gcc/x86_64-elf/12.1.0
#CC = clang
#LD = ld
#CFLAGS += --target=x86_64-elf
#LD_FLAGS += --target=x86_64-elf
		endif
	endif
endif

.PHONY: all clean objlib help shell

all: 

objlib: LD_FLAGS += -r
objlib: EXE = $(EXE_BASE_NAME).o
objlib: $(EXE_BASE_NAME).o

$(EXE_BASE_NAME).o: $(OBJ) $(DEP_OBJ)
	$(LD) $(LD_FLAGS) -o $(EXE) $+

$(EXE): $(OBJ) $(DEP_OBJ)
	$(LD) $(LD_FLAGS) -o $@ $+

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

help:
	echo "\
	\n\nHELP MENU\n \
	options: \n \
	host=<your distribution>_<your CPU architecture>\n \
	    following are supported:\n \
	    macos_arm     | \n \
	    macos_arm64   | These have \n \
	    macos_aarch64 | the same meaning\n \
	    macos_m1      | \n\n \
	\
	    macos_i386   | \n \
	    macos_x86    | These have the same meaning\n \
	    macos_x86_32 | \n\n \
	\
	    macos_x86_64 \n\n \
	\
	    linux_arm     | \n \
	    linux_arm64   | These have \n \
	    linux_aarch64 | the same meaning\n \
	    linux_m1      | \n\n \
	\
	    linux_i386   | \n \
	    linux_x86    | These have the same meaning\n \
	    linux_x86_32 | \n\n \
	\
	    linux_x86_64 \n\n \
	\
	    windows_arm     | \n \
	    windows_arm64   | These have \n \
	    windows_aarch64 | the same meaning\n \
	    windows_m1      | \n\n \
	\
	    windows_i386   | \n \
	    windows_x86    | These have the same meaning\n \
	    windows_x86_32 | \n\n \
	\
	    windows_x86_64 \n\n \
	target=<your distribution>_<your CPU architecture>\n \
	    supports the same as host=... (above) \n\n \
	help \n \
		prints this help screen \n\n \
	clean \n \
		removes all object files (.o) \n\n \
	all \n \
	    default action - nothing for the moment \n \
	    This will also be done if you simply type make without arguments \n\n \
	objlib \n \
	    Creates an msh.o file, which can be linked with another c or cpp file, \
to use the msh interpreter in your own c program. \n \
	    To include it in your c program you also have to include a header file (mshgit/include/msh.h) \n\n \
	shell \n \
	    This will soon be available. \n\n \
	"
-include $(OBJ:.o=.d)