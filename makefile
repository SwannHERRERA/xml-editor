BUILD_DIR = build/
CC = gcc
EXTRAFLAG = -std=c18 -Wall -Wextra -Werror
RMDIR = rm -rf

create_dir:
	mkdir -p $(BUILD_DIR)
clean:
	@echo "Deleting $(BUILD_DIR) files"
	$(RMDIR) $(BUILD_DIR)

all: dev

dev: build start

build: create_dir
	$(CC) $(EXTRAFLAG) -o $(BUILD_DIR)executable.out main.c 
start:
	$(BUILD_DIR)executable.out $(inputFile)