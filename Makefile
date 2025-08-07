BIN := neat
MAIN_FILE := src/main.c
C_FILES := $(shell find src -name '*.c' ! -name 'main.c')
H_FILES := $(shell find src -name '*.h')

TEST_FILES := $(shell find tests -name '*.c')
TEST_BINS := $(patsubst tests/%.c,bin/tests/%,${TEST_FILES})

all: ${C_FILES} ${H_FILES} ${MAIN_FILE}
	mkdir -p bin
	gcc -o bin/${BIN} ${C_FILES} ${MAIN_FILE}

tests: bin/tests ${TEST_BINS}
	@echo all tests build

bin/tests:
	mkdir -p bin/tests

bin/tests/%: tests/%.c ${C_FILES} ${H_FILES}
	gcc -Isrc -o $@ $< ${C_FILES}

clean:
	rm -rf bin

.PHONY: clean tests
