# Installed as `Makefile` by setup.sh.
#
# Usage:  make f=main        build main.cpp (sanitizers, debug(), file I/O)
#         make r f=main      build + run + compare with expected_output.txt
#         make pipe f=main   build + run reading stdin instead
#         make fast f=main   submission flags (no sanitizers, no debug)
#         make listen        fill input/expected from Competitive Companion
#         make sample n=2    load another parsed sample
#         make clean
#
# f defaults to main, and may include a subfolder: make r f=contests/div2/a

f   ?= main
CXX ?= g++

STD  := -std=c++23
INC  := -Ilib
WARN := -Wall -Wextra -Wshadow -Wno-sign-compare

SAN   := -fsanitize=address,undefined -fno-sanitize-recover=all
BASE  := $(STD) $(INC) $(WARN) -O2 -g -D_UTSAV_ -D_GLIBCXX_DEBUG $(SAN)

DEBUG_FLAGS := $(BASE) -DUSE_FILES
PIPE_FLAGS  := $(BASE)
FAST_FLAGS  := $(STD) $(INC) -O2

BIN := build/$(f)

.PHONY: all r pipe fast clean listen sample

all: $(BIN)

$(BIN): $(f).cpp
	@mkdir -p $(dir $(BIN))
	$(CXX) $(DEBUG_FLAGS) -o $@ $<

# what the run button calls
r: $(BIN)
	@tools/check.sh $(BIN)

pipe:
	@mkdir -p $(dir $(BIN))
	$(CXX) $(PIPE_FLAGS) -o $(BIN) $(f).cpp
	@./$(BIN)

fast:
	@mkdir -p $(dir $(BIN))
	$(CXX) $(FAST_FLAGS) -o $(BIN) $(f).cpp

# wait for Competitive Companion and fill input.txt / expected_output.txt
listen:
	@python3 tools/listen.py

# load another parsed sample:  make sample n=2
n ?= 1
sample:
	@cp tests/$(n).in input.txt
	@cp tests/$(n).out expected_output.txt
	@echo "loaded sample $(n) into input.txt / expected_output.txt"

clean:
	@rm -rf build
