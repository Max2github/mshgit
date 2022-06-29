# $@ ... target
# $? ... all dependencies - not sure
# $< ... first dependency
# $^ ... all dependencies - no duplicates
# $+ ... all dependencies - including duplicates

CC = gcc # compiler
LD = gcc #ld # linker
CFLAGS = -c -Wall #-std=c++20 # flags for compiler
LD_FLAGS =  # flags for linker
# executable / target file
EXE_BASE_NAME := msh
EXE = $(EXE_BASE_NAME)

CROSS_CC_WIN_x86_32 := i686-w64-mingw32-gcc
CROSS_CC_WIN_x86_64 := x86_64-w64-mingw32-gcc
CROSS_CC_LINUX_x86_32 := x86_64-unknown-linux-gnu-gcc
CROSS_CC_LINUX_i386 := x86_64-unknown-linux-gnu-gcc
CROSS_CC_LINUX_x86_64 := x86_64-unknown-linux-gnu-gcc
CROSS_CC_LINUX_ARM64 := aarch64-unknown-linux-gnu-gcc
CROSS_CC_MAC_x86_64 := $(CC)
CROSS_CC_MAC_ARM64 := $(CC)

CROSS_LD_WIN_x86_32 := i686-w64-mingw32-ld
CROSS_LD_WIN_x86_64 := x86_64-w64-mingw32-ld
CROSS_LD_LINUX_x86_32 := x86_64-unknown-linux-gnu-ld
CROSS_CC_LINUX_i386 := x86_64-unknown-linux-gnu-ld
CROSS_LD_LINUX_x86_64 := x86_64-unknown-linux-gnu-ld
CROSS_LD_LINUX_ARM64 := aarch64-unknown-linux-gnu-ld
CROSS_LD_MAC_x86_64 := $(LD)
CROSS_LD_MAC_ARM64 := $(LD)

EXE_BUILD_DIR := build

EXE_RELEASE_MACOS_ARM64 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_mach-o_arm64
EXE_RELEASE_MACOS_x86_64 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_mach-o_x86_64
EXE_RELEASE_LINUX_ARM64 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_linux_arm64
EXE_RELEASE_LINUX_x86_32 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_linux_x86_32
EXE_RELEASE_LINUX_x86_64 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_linux_x86_64
EXE_RELEASE_WINDOWS_x86_32 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_win_x86_32.exe
EXE_RELEASE_WINDOWS_x86_64 := $(EXE_BUILD_DIR)/$(EXE_BASE_NAME)_win_x86_64.exe

MEMCHECK = leaks
MEMCHECK_FLAGS = --atExit -- 
# MEMCHECK = valgrind
# MEMCHECK_FLAGS = 

DEVELOP_EXE := devel
DEVELOP_DIR := develop
DEVELOP_SRC := $(DEVELOP_DIR)/develop.c

SRC_DIR := src
OBJ_DIR := o
INC_DIR := include

ifdef dep
	ifeq ($(dep), false)
# set to none - this may generate an error
DEP := 
DEP_DIR := none
DEP_OBJ := $(OBJ_DIR)/lib/all.o
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
# linux_x86_64   | ✔
# linux_x86_32   | x

HOST_OS = macos
HOST_ARCH = arm

ifdef host
	ifneq (,$(filter $(host),macos_i386 macos_x86_32 macos_x86 macos_x86_64))
		HOST_OS = macos
		HOST_ARCH = x86_64
	endif
	ifneq (,$(filter $(host),macos_arm macos_arm64 macos_aarch64 macos_m1))
		HOST_OS = macos
		HOST_ARCH = arm
	endif
	ifneq (,$(filter $(host),linux_i386 linux_x86_32 linux_x86))
		MEMCHECK = valgrind
		MEMCHECK_FLAGS = --leak-check=full
		CROSS_CC_LINUX_x86_32 = gcc
		CROSS_LD_LINUX_x86_32 = gcc
		CROSS_CC_LINUX_i386 = gcc
		CROSS_LD_LINUX_i386 = gcc
		CROSS_CC_LINUX_x86_64 = x86_64-linux-gnu-gcc
		CROSS_LD_LINUX_x86_64 = x86_64-linux-gnu-ld
		HOST_OS = linux
		HOST_ARCH = arm
	endif
	ifeq ($(host), linux_x86_64)
		CROSS_CC_LINUX_x86_32 = gcc
		CROSS_LD_LINUX_x86_32 = gcc
		CROSS_CC_LINUX_x86_64 = gcc
		CROSS_LD_LINUX_x86_64 = gcc
	endif
	ifneq (,$(filter $(host),linux_arm linux_arm64 linux_aarch64 linux_m1))
		MEMCHECK = valgrind
		MEMCHECK_FLAGS = --leak-check=full
		CROSS_CC_LINUX_x86_32 = x86_64-linux-gnux32-gcc
		CROSS_LD_LINUX_x86_32 = x86_64-linux-gnux32-gcc
		CROSS_CC_LINUX_i386 = i686-linux-gnu-gcc
		CROSS_LD_LINUX_i386 = i686-linux-gnu-gcc
#x86_64-linux-gnu-ld -m elf_i386 #-L /usr/lib/gcc-cross/x86_64-linux-gnu/9/ -lgcc #x86_64-linux-gnux32-ld
		CROSS_CC_LINUX_x86_64 = x86_64-linux-gnu-gcc
		CROSS_LD_LINUX_x86_64 = x86_64-linux-gnu-ld
		CROSS_CC_LINUX_ARM64 = gcc
		HOST_OS = linux
		HOST_ARCH = arm
	endif
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
		ifneq (, $(filter $(HOST_OS), macos, linux))
#ifeq ($(host), $(filter linux, macos))
#CC = aarch64-none-elf-gcc
			CC = $(CROSS_CC_LINUX_ARM64)
			LD = $(CROSS_LD_LINUX_ARM64)
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
		ifneq (, $(filter $(HOST_OS), macos linux))
#export PATH="/usr/local/Cellar/x86_64-elf-binutils/2.38/bin/:/usr/local/Cellar/x86_64-elf-gcc/12.1.0/bin/:/usr/local/Cellar/i386-elf-gdb/12.1/bin:$PATH"
#CC = x86_64-elf-gcc
#LD = x86_64-elf-ld
			ifneq ($(target), linux_i386)
				CC = $(CROSS_CC_LINUX_i386)
				LD = $(CROSS_LD_LINUX_i386)
			else
				CC = $(CROSS_CC_LINUX_i386)
				LD = $(CROSS_LD_LINUX_i386)
			endif
#CFLAGS += -m64 #--sysroot=/usr/local/Cellar/x86_64-elf-gcc/12.1.0/
#LD_FLAGS += -m elf_x86_64
		endif
	endif

	ifneq (,$(filter $(target),windows_i386 windows_x86_32 windows_x86))
# macos : macports : i686-w64-mingw32-gcc, x86_64-w64-mingw32-gcc
		ifneq (, $(filter $(HOST_OS), macos linux))
			CC = $(CROSS_CC_WIN_x86_32)
			LD = $(CROSS_LD_WIN_x86_32)
		endif
	endif
	ifeq ($(target), windows_x86_64)
		ifneq (, $(filter $(HOST_OS), macos linux))
			CC = $(CROSS_CC_WIN_x86_64)
			LD = $(CROSS_LD_WIN_x86_64)
		endif
	endif
	ifeq ($(target), macos_x86_64)
		ifeq ($(HOST_OS), macos)
			CC = $(CROSS_CC_MAC_x86_64)
			LD = $(CROSS_CC_MAC_x86_64)
			CFLAGS += --target=x86_64-apple-darwin-macho
			CLD_FLAGS += --target=x86_64-apple-darwin-macho
		endif
	endif
	ifeq ($(target), linux_x86_64)
		ifneq (, $(filter $(HOST_OS), macos linux))
			CC = $(CROSS_CC_LINUX_x86_64)
			LD = $(CROSS_LD_LINUX_x86_64)
#CC = x86_64-elf-gcc
#LD = x86_64-elf-ld
#CFLAGS += -m64 -I lib/gcc/x86_64-elf/12.1.0/include
#LD_FLAGS += -m elf_x86_64 -L lib/gcc/x86_64-elf/12.1.0
#CC = clang
#LD = ld
#CFLAGS += --target=x86_64-elf
#LD_FLAGS += --target=x86_64-elf
		endif
	endif
endif

ifndef file
	file := 
endif

.PHONY: all # default: do nothing
.PHONY: clean cleanshell cleanexe_win cleanexe cleanbuild # cleaning up
.PHONY: objlib shell release # building
.PHONY: develop # build development tools
.PHONY: command # use development tools
.PHONY: help # help screen
.PHONY: memcheck_shell # testing

all: 

develop: $(DEVELOP_EXE)

memcheck_shell:
	$(MEMCHECK) $(MEMCHECK_FLAGS) ./$(EXE) $(file)

objlib: LD_FLAGS += -r
objlib: EXE = $(EXE_BASE_NAME).o
objlib: $(EXE_BASE_NAME).o

#shell: LD = $(CC)
shell: $(EXE)

shell.o: shell.c
	$(CC) $(CFLAGS) -o $@ $^

release:
	mkdir build
release: $(EXE_RELEASE_LINUX_ARM64) $(EXE_RELEASE_LINUX_x86_64)
#release : $(EXE_RELEASE_LINUX_x86_32)
#release: $(EXE_RELEASE_MACOS_ARM64)
#release: $(EXE_RELEASE_MACOS_x86_64)
release: $(EXE_RELEASE_WINDOWS_x86_32) $(EXE_RELEASE_WINDOWS_x86_64)
release: $(EXE) # nativ -> in this case for macos

# release
$(EXE_RELEASE_LINUX_ARM64):
	make target=linux_arm shell
	mv $(EXE) $@
	make cleanshell
$(EXE_RELEASE_LINUX_x86_32):
	make target=linux_x86_32 shell
	mv $(EXE) $@
	make cleanshell
$(EXE_RELEASE_LINUX_x86_64):
	make target=linux_x86_64 shell
	mv $(EXE) $@
	make cleanshell
$(EXE_RELEASE_MACOS_ARM64):
	make target=macos_arm shell
	mv $(EXE) $@
	make cleanshell
$(EXE_RELEASE_MACOS_x86_64):
	make target=macos_x86_64 shell
	mv $(EXE) $@
	make cleanshell
$(EXE_RELEASE_WINDOWS_x86_32): EXE = $(EXE_BASE_NAME).exe
$(EXE_RELEASE_WINDOWS_x86_32):
	make target=windows_x86_32 shell
	mv $(EXE) $@
	make cleanshell
$(EXE_RELEASE_WINDOWS_x86_64): EXE = $(EXE_BASE_NAME).exe
$(EXE_RELEASE_WINDOWS_x86_64):
	make target=windows_x86_64 shell
	mv $(EXE) $@
	make cleanshell

# objlib
$(EXE_BASE_NAME).o: $(OBJ) $(DEP_OBJ)
	$(LD) $(LD_FLAGS) -o $(EXE) $+

# shell
$(EXE): $(OBJ) $(DEP_OBJ) shell.o
	echo "$(LD)"
	$(LD) $(LD_FLAGS) -o $@ $^

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

cleanshell: clean
	rm shell.o

cleanexe_win:
	rm $(EXE).exe

cleanexe:
	rm $(EXE)

cleanbuild:
	rm $(EXE_BASE_NAME).o

$(DEVELOP_EXE): $(DEVELOP_SRC) $(DEP_OBJ)
	$(CC) -o $@ $^

command: develop
	./$(DEVELOP_EXE) $(action) $(data)

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