BIN := neat
MAIN_FILE := src/main.c
C_FILES := $(shell find src -name '*.c' ! -name 'main.c')
H_FILES := $(shell find src -name '*.h')

TEST_FILES := $(shell find tests -name '*.c')

all: ${C_FILES} ${H_FILES} ${MAIN_FILE}
	mkdir -p bin
	gcc -o bin/${BIN} ${C_FILES} ${MAIN_FILE}

tests: 
	@mkdir -p bin/tests
	@for file in ${TEST_FILES}; do \
		bin=$${file%.c}; \
		echo building $$file; \
		gcc -Isrc -o bin/$$bin $$file ${C_FILES}; \
	done


clean:
	rm -rf bin

.PHONY: clean tests
