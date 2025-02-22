RAYLIB_PATH=C:\raylib\raylib
COMPILER_PATH ?= C:\raylib\w64devkit\bin
export PATH := $(COMPILER_PATH):$(PATH)

ifdef BUILD_SERVER
	PROJECT := server.exe
	BUILD_DIR = server/bin/$(PROJECT)

	DIR_SRC += ./server
	DIR_SRC += ./server/src
else
	PROJECT := game.exe
	BUILD_DIR = client/bin/$(PROJECT)
	
	DIR_SRC += ./client
	DIR_SRC += ./client/src
	DIR_SRC += ./client/src/chunk
	DIR_SRC += ./client/src/block
	DIR_SRC += ./client/src/entity
	DIR_SRC += ./client/src/gui
	DIR_SRC += ./client/src/networking
endif

DIR_INC = $(addprefix -I, $(DIR_SRC))

SRC_C += $(wildcard $(addsuffix /*.c, $(DIR_SRC)))
OBJ := $(patsubst %.c, %.o, $(SRC_C))

CC := gcc
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces
ifdef DEBUG
	CFLAGS += -g -Og
else
	CFLAGS += -s -O2 -Wl,--subsystem,windows
endif

INCLUDE_PATHS = -I$(DIR_INC) -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I$(RAYLIB_PATH)/src/extras
LDFLAGS = -L. -L$(RAYLIB_PATH)/src
LDLIBS = -static -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread -lws2_32 -lwinmm

.PHONY:all

all:
	mingw32-make $(PROJECT)

$(PROJECT): $(OBJ)
	$(CC) -o $(BUILD_DIR) $(OBJ) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -DPLATFORM_DESKTOP

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -DPLATFORM_DESKTOP