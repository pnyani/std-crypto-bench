# Tools
CC     := gcc
PYTHON := python3

# -mno-aes: disable AES-NI so all algorithms are compared as software only
# -fno-inline -fno-ipa-cp -fno-ipa-sra -fno-optimize-sibling-calls: reproducible Ir across GCC versions
CFLAGS   := -std=c99 -O2 -mno-aes -march=x86-64 \
            -fno-inline -fno-ipa-cp               \
            -fno-ipa-sra                          \
            -fno-optimize-sibling-calls           \
            -fno-tree-loop-vectorize              \
            -fno-tree-slp-vectorize               \
            -fno-ivopts                           \
            -Wall -Wextra

LDFLAGS  := -static

INCLUDES := -Iinclude -Iinclude/common

ALGO_SRCS := \
    src/aes.c         \
    src/aes_variant.c \
    src/aria.c        \
    src/seed.c        \
    src/des.c         \
    src/3des.c

BENCH_SRCS := bench/harness.c bench/main.c $(ALGO_SRCS)

TEST_SRCS := \
    test/test_runner.c      \
    test/test_aes.c         \
    test/test_aes_variant.c \
    test/test_aria.c        \
    test/test_seed.c        \
    test/test_des.c         \
    $(ALGO_SRCS)

.PHONY: all test valgrind clean

all: bin/bench


bin:
	mkdir -p bin

bin/bench: $(BENCH_SRCS) | bin
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $^

valgrind: bin/bench
	valgrind \
	    --tool=callgrind \
	    --cache-sim=no   \
	    --branch-sim=no  \
	    --callgrind-out-file=callgrind_bench.out \
	    ./bin/bench
	callgrind_annotate callgrind_bench.out

test: test_runner
	./test_runner

test_runner: $(TEST_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -Itest $(LDFLAGS) -o $@ $^

clean:
	rm -f bin/bench test_runner callgrind.out.* callgrind_*.out
