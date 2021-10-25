# Travis example for Identifier created by Rafael Garibotti

GCCFLAGS = -g -Wall -Wfatal-errors 
ALL = identifier
GCC = gcc

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition

ifeq (run_code,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

# all: $(ALL)

# identifier: ./identifier.c
# 	$(GCC) $(GCCFLAGS) -o $@ $@.c

# cov: ./identifier.c
# 	$(GCC) $(GCCFLAGS) -fprofile-arcs -ftest-coverage -o $@ identifier.c

# clean:
# 	rm -fr $(ALL) *.o cov* *.dSYM *.gcda *.gcno *.gcov

# test: all
# 	bash test
run: 
	make run_unity
	@echo "----------GCOV----------"
	@make compile_gcov
	@make run_test_gcov
	@lcov --capture --directory ./ --output-file coverage.info
	@mkdir ./logs/coverage
	@genhtml coverage.info --output-directory ./logs/coverage
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov identifier_pro
	make run_valgrind
	make run_sanitizer

clean: 
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov identifier_pro logs

# COMPILAR E RODAR CÓDIGO

run_code:
	@echo "----------CPP CHECK----------"
	cppcheck --enable=all /identifier/src/main_identifier.c ./identifier/src/identifier.c
	@echo "----------COMPILING WITH ARGUMENT----------"
	gcc -Wall -fprofile-arcs -ftest-coverage ./identifier/src/main_identifier.c ./identifier/src/identifier.c -o ./identifier/src/identifier_pro
	@echo "----------RUNNING----------"
	./identifier/src/identifier_pro $(RUN_ARGS)
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov ./identifier/src/identifier_pro

# GCOV AND LCOV

compile_gcov:
	cppcheck --enable=all /identifier/src/main_identifier.c ./identifier/src/identifier.c
	gcc -Wall -fprofile -fprofile-arcs -ftest-coverage ./identifier/src/main_identifier.c ./identifier/src/identifier.c -o identifier_pro

run_test_gcov:
	@mkdir logs
	@echo "Executando Teste 1 -> Entrada: teste"
	@echo "teste" | ./identifier_pro teste
	@gcov identifier.gcno -b >> ./logs/log_teste1
	@mv identifier.c.gcov ./logs/log_gcov_teste1
	@echo "Executando Teste 2 -> Entrada: 4abc"
	@echo "4abc" | ./identifier_pro 4abc -i
	@gcov identifier.gcno -b >> ./logs/log_teste2
	@mv identifier.c.gcov ./logs/log_gcov_teste2
	@echo "Executando Teste 3 -> Entrada: ab!"
	@echo "ab!" | ./identifier_pro ab!
	@gcov identifier.gcno -b >> ./logs/log_teste3
	@mv identifier.c.gcov ./logs/log_gcov_teste3
	@echo "Executando Teste 4 -> Entrada: "
	@echo "teste" | ./identifier_pro
	@gcov identifier.gcno -b >> ./logs/log_teste4
	@mv identifier.c.gcov ./logs/log_gcov_teste4

run_gcov:
	@echo "----------GCOV----------"
	@make compile_gcov
	@make run_test_gcov
	@lcov --capture --directory ./ --output-file coverage.info
	@mkdir ./logs/coverage
	@genhtml coverage.info --output-directory ./logs/coverage
	@firefox ./logs/coverage/index.html
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov identifier_pro

# VALGRIND (REVER SE NECESSITA GCC)
run_valgrind:  
	gcc -Wall -fprofile -fprofile-arcs -ftest-coverage ./identifier/src/main_identifier.c ./identifier/src/identifier.c -o identifier_pro
	mkdir ./logs/valgrind
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/valid ./identifier_pro t4ste
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/invalid ./identifier_pro t%ste
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov identifier_pro

run_sanitizer:
	gcc -g -Wall -Wfatal-errors -fsanitize=address -fsanitize=leak ./identifier/src/main_identifier.c ./identifier/src/identifier.c -o identifier_pro
	mkdir ./logs/sanitizer
	./identifier_pro t4ste 2> ./logs/sanitizer/valid
	./identifier_pro testeste 2> ./logs/sanitizer/invalid
	rm -fr identifier_pro

# UNITY
UNITY_ROOT=Unity
IDENTIFIER_ROOT=identifier
TARGET1 = all_tests
SRC_FILES1=\
  $(UNITY_ROOT)/src/unity.c \
  $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
  $(IDENTIFIER_ROOT)/src/identifier.c \
  $(IDENTIFIER_ROOT)/test/test_identifier.c \
  $(IDENTIFIER_ROOT)/test/test_runners/test_identifier_runner.c \
  $(IDENTIFIER_ROOT)/test/test_runners/all_tests.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src -I$(UNITY_ROOT)/extras/fixture/src

run_unity: 
	gcc $(CFLAGS) $(INC_DIRS) $(SRC_FILES1) -o $(TARGET1)
	- ./$(TARGET1) -v
	rm -fr all_tests

# RODAR TUDO
run_all:
	make run_unity
	make run_gcov
	make run_valgrind
	make run_sanitizer
	make run_code teste
