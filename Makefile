BIN := neat
C_FILES := $(shell find src -name '*.c')
H_FILES := $(shell find src -name '*.h')

all: ${C_FILES} ${H_FILES}
	mkdir -p bin
	gcc -o bin/${BIN} ${C_FILES}

.PHONY: clean
clean:
	rm -rf bin
