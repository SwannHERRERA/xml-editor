BUILD_DIR = build/
CC = gcc
EXTRAFLAG = -std=c18 -Wall -Wextra -Werror
RMDIR = rm -rf
SRC = src/

create_dir:
	mkdir -p $(BUILD_DIR)
clean:
	@echo "Deleting $(BUILD_DIR) files"
	$(RMDIR) $(BUILD_DIR)

all: dev

dev: build start

build: create_dir
	$(CC) $(EXTRAFLAG) -o $(BUILD_DIR)executable.out $(SRC)file_helper.c $(SRC)xml_element.c $(SRC)parse_dtd.c $(SRC)parse_xml.c $(SRC)main.c 
start:
	$(BUILD_DIR)executable.out $(inputFile)