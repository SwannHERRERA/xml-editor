BUILD_DIR = build/
CC = gcc
EXTRAFLAG = -std=c18 -Wall -Wextra  -ggdb
RMDIR = rm -rf
SRC = src/
LIB= `pkg-config gtk+-3.0 --libs --cflags`
LIB+= `pkg-config gmodule-2.0 --libs`

create_dir:
	mkdir -p $(BUILD_DIR)
clean:
	@echo "Deleting $(BUILD_DIR) files"
	$(RMDIR) $(BUILD_DIR)

all: dev

dev: build start

build: create_dir
	$(CC) $(EXTRAFLAG) -o $(BUILD_DIR)executable.out $(SRC)file_helper.c $(SRC)xml_element.c $(SRC)parse_dtd.c $(SRC)parse_xml.c $(SRC)check_xml_corresponding.c $(SRC)interface.c $(SRC)main.c $(LIB)
start:
	$(BUILD_DIR)executable.out $(inputFile)