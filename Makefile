IDIR = .
CC = cc

ifdef MODERN_CC
	EXTRA_C_FLAGS = -g -O03 -std=c99 -pedantic -Wall -Wno-unused-function
endif

ifdef CC_32_BIT
	EXTRA_C_FLAGS = -m32 -g -O03 -std=c99 -pedantic -Wall -Wno-unused-function
endif

ifdef ADD_SAN
	CC = clang
	EXTRA_C_FLAGS = -std=c99 -Wall -pedantic -g -O00 -fsanitize=address -fno-omit-frame-pointer -Wno-unused-function
endif

ifdef MEM_SAN
	CC = clang
	EXTRA_C_FLAGS = -std=c99 -Wall -pedantic -g -O00 -fsanitize=memory -fno-omit-frame-pointer -Wno-unused-function
endif

ifdef UB_SAN
	CC = clang
	EXTRA_C_FLAGS = -std=c99 -Wall -pedantic -g -O00 -fsanitize=undefined -fno-omit-frame-pointer -Wno-unused-function
endif

CFLAGS =  -g -O00 -I$(IDIR) $(EXTRA_C_FLAGS)

ODIR = .
LDIR =

_DEPS = dynarr.h test_framework.h hashbag.h lru.h list.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = test_dynarr.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
C_FILES = $(patsubst %.o,%.c,$(_OBJ))

_OBJ2 = test_hashbag.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))
C_FILES2 = $(patsubst %.o,%.c,$(_OBJ2))

_OBJ3 = test_lru.o
OBJ3 = $(patsubst %,$(ODIR)/%,$(_OBJ3))
C_FILES3 = $(patsubst %.o,%.c,$(_OBJ3))

ALL_CFILES = $(C_FILES) $(C_FILES2) $(C_FILES3)

main: test_dynarr.bin test_hashbag.bin test_lru.bin
	echo OK

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test_dynarr.bin: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

test_hashbag.bin: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS)

test_lru.bin: $(OBJ3)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean test run_test_continusly CMakeLists.txt cmake_compile clang_format test_add_san test_ub_san test_mem_san test_sanitizers test_modern_cc test_valgrind test_macro_expanded.bin

test_macro_expanded_gen:
	gcc -E -C -P $(TESTPROG) > tmp_tmp_test.c
	clang-format tmp_tmp_test.c > tmp_test.c
	rm tmp_tmp_test.c
	gcc -Wall -Wno-unused-function  -g -O01 tmp_test.c
	valgrind --leak-check=full ./a.out

test_macro_expanded_dynarr:
	TESTPROG=test_dynarr.c make test_macro_expanded_gen

test_macro_expanded_hashbag:
	TESTPROG=test_hashbag.c make test_macro_expanded_gen

test_macro_expanded_lru:
	TESTPROG=test_lru.c make test_macro_expanded_gen

test: test_dynarr.bin test_hashbag.bin test_lru.bin
	./test_dynarr.bin && ./test_hashbag.bin && ./test_lru.bin;\
	RESULT=$$? &&\
	(exit $$RESULT) &&\
	printf "\n\n\033[0;32mALL TESTS PASSED!\033[0m\n\n\n" ||\
	printf "\n\n\033[0;31mTEST FAILED!\033[0m\n\n\n" &&\
	exit $$RESULT

test_valgrind_gen:
	make clean && \
	make EXTRA_C_FLAGS="-g -O01" && \
	valgrind --undef-value-errors=no --leak-check=full $(BIN) ;\
	RESULT=$$? &&\
	(exit $$RESULT) &&\
	printf "\n\n\033[0;32mALL TESTS PASSED!\033[0m\n\n\n" ||\
	printf "\n\n\033[0;31mTEST FAILED!\033[0m\n\n\n" &&\
	exit $$RESULT

test_valgrind:
	BIN=./test_dynarr.bin make test_valgrind_gen
	BIN=./test_hashbag.bin make test_valgrind_gen
	BIN=./test_lru.bin make test_valgrind_gen

test_add_san:
	make clean && \
	make ADD_SAN=1 test

test_mem_san:
	make clean && \
	make MEM_SAN=1 test

test_ub_san:
	make clean && \
	make UB_SAN=1 test

test_sanitizers:
	make test_add_san && \
	make test_mem_san && \
	make test_ub_san

test_modern_cc:
	make clean && \
	make MODERN_CC=1 test

test_32_bit:
	make clean && \
	make CC_32_BIT=1 test && \
	make clean

test_all:
	make test_valgrind && \
	make test_sanitizers && \
	make test_modern_cc
# make test_32_bit

run_test_continusly:
	inotifywait -e close_write,moved_to,create -m ./*.c ./*.h | while read -r directory events filename; do gtags ; make test ; done

CMakeLists.txt: $(C_FILES) $(C_FILES2) $(C_FILES3) 
	echo "cmake_minimum_required (VERSION 2.6)" > CMakeLists.txt
	echo "project (DS)" >> CMakeLists.txt
	echo "add_executable(dynarr.out" >> CMakeLists.txt
	echo $(C_FILES) >> CMakeLists.txt
	echo ")" >> CMakeLists.txt
	echo "add_executable(hashbag.out" >> CMakeLists.txt
	echo $(C_FILES2) >> CMakeLists.txt
	echo ")" >> CMakeLists.txt
	echo "add_executable(lru.out" >> CMakeLists.txt
	echo $(C_FILES3> CMakeLists.txt
	echo ")" >> CMakeLists.txt


cmake_compile: CMakeLists.txt
	mkdir cmake_mkdir || true
	cd cmake_mkdir && cmake ..

clang_tidy:
	ls *.c | xargs -I{} -n1 clang-tidy -warnings-as-errors=*  {} -- $(CFLAGS)

clang_format:
	clang-format -style="{BasedOnStyle: LLVM}" -i *.c *.h

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ test_dynarr.bin CMakeLists.txt
