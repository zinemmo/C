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

ifeq (run_all,$(firstword $(MAKECMDGOALS)))
  RUN_ALL_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ALL_ARGS):;@:)
endif

run: 
	make run_unity
	@make compile_gcov
	@make run_test_gcov
	@lcov --capture --directory ./ --output-file coverage.info
	@mkdir -p ./logs/coverage
	@genhtml coverage.info --output-directory ./logs/coverage
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov ./crypto/src/main
	make run_valgrind
	make run_sanitizer

clean: 
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov main logs ./crypto/src/main

# COMPILAR E RODAR CÓDIGO
run_code:
	@echo "----------CPP CHECK----------"
	cppcheck --enable=all --check-config crypto/src/main.c crypto/src/aes.c crypto/src/blowfish.c crypto/src/crypt.c crypto/src/xtea.c
	@echo "----------COMPILING WITH ARGUMENT----------"
	gcc -Wall -fprofile-arcs -ftest-coverage crypto/src/main.c crypto/src/aes.c crypto/src/blowfish.c crypto/src/crypt.c crypto/src/xtea.c -o ./crypto/src/main
	@echo "----------RUNNING----------"
	./crypto/src/main $(RUN_ARGS)
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov ./crypto/src/main

# GCOV AND LCOV
compile_gcov:
	cppcheck --enable=all crypto/src/main.c crypto/src/aes.c crypto/src/blowfish.c crypto/src/crypt.c crypto/src/xtea.c
	gcc -Wall -fprofile -fprofile-arcs -ftest-coverage ./crypto/src/main.c crypto/src/aes.c crypto/src/blowfish.c crypto/src/crypt.c crypto/src/xtea.c -o ./crypto/src/main

run_test_gcov:
	@mkdir logs
	#type = 0 -> XTEA
	@echo "Executando Teste 1 -> Entrada: 0" 
	./crypto/src/main 0
	@gcov xtea.gcno -b >> ./logs/log_xtea
	@mv xtea.c.gcov ./logs/log_gcov_xtea
	
	#type = 1 -> AES-128
	@echo "Executando Teste 2 -> Entrada: 1"
	./crypto/src/main 1
	@gcov aes.gcno -b >> ./logs/log_aes_128
	@mv aes.c.gcov ./logs/log_gcov_aes_128
	
	#type = 2 -> AES-192
	@echo "Executando Teste 3 -> Entrada: 2"
	./crypto/src/main 2
	@gcov aes.gcno -b >> ./logs/log_aes_192
	@mv aes.c.gcov ./logs/log_gcov_aes_192
	
	#type = 3 -> AES-256
	@echo "Executando Teste 4 -> Entrada: 3"
	./crypto/src/main 3
	@gcov aes.gcno -b >> ./logs/log_aes_256
	@mv aes.c.gcov ./logs/log_gcov_aes_256
	
	#type = 4 -> BLOWFISH-128
	@echo "Executando Teste 5 -> Entrada: 4"
	./crypto/src/main 4
	@gcov blowfish.gcno -b >> ./logs/log_blowfish_128
	@mv blowfish.c.gcov ./logs/log_gcov_blowfish_128
	
	#type = 5 -> BLOWFISH-192
	@echo "Executando Teste 6 -> Entrada: 5"
	./crypto/src/main 5
	@gcov blowfish.gcno -b >> ./logs/log_blowfish_192
	@mv blowfish.c.gcov ./logs/log_gcov_blowfish_192
	
	#type = 6 -> BLOWFISH-256
	@echo "Executando Teste 7 -> Entrada: 6"
	./crypto/src/main 6
	@gcov blowfish.gcno -b >> ./logs/log_blowfish_256
	@mv blowfish.c.gcov ./logs/log_gcov_blowfish_256

run_gcov:
	@echo "----------GCOV----------"
	@make compile_gcov
	@make run_test_gcov
	@lcov --capture --directory ./ --output-file coverage.info
	@mkdir -p ./logs/coverage
	@genhtml coverage.info --output-directory ./logs/coverage
	@firefox ./logs/coverage/index.html
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov ./crypto/src/main

# VALGRIND (REVER SE NECESSITA GCC)
run_valgrind:  
	gcc -Wall -fprofile -fprofile-arcs -ftest-coverage ./crypto/src/main.c crypto/src/aes.c crypto/src/blowfish.c crypto/src/crypt.c crypto/src/xtea.c -o ./crypto/src/main
	mkdir -p ./logs/valgrind
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type0 ./crypto/src/main 0
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type1 ./crypto/src/main 1
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type2 ./crypto/src/main 2
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type3 ./crypto/src/main 3
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type4 ./crypto/src/main 4
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type5 ./crypto/src/main 5
	valgrind --leak-check=full --show-leak-kinds=all --log-file=./logs/valgrind/type6 ./crypto/src/main 6
	rm -fr *.o cov* *.dSYM *.gcda *.gcno *.gcov ./crypto/src/main

run_sanitizer:
	gcc -g -Wall -Wfatal-errors -fsanitize=address -fsanitize=leak ./crypto/src/main.c crypto/src/aes.c crypto/src/blowfish.c crypto/src/crypt.c crypto/src/xtea.c -o ./crypto/src/main
	mkdir -p ./logs/sanitizer
	./crypto/src/main 0 2> ./logs/sanitizer/type0
	./crypto/src/main 1 2> ./logs/sanitizer/type1
	./crypto/src/main 2 2> ./logs/sanitizer/type2
	./crypto/src/main 3 2> ./logs/sanitizer/type3
	./crypto/src/main 4 2> ./logs/sanitizer/type4
	./crypto/src/main 5 2> ./logs/sanitizer/type5
	./crypto/src/main 6 2> ./logs/sanitizer/type6
	rm -fr ./crypto/src/main

# UNITY
UNITY_ROOT=Unity
CRYPTO_ROOT=crypto
TARGET1 = all_tests
SRC_FILES1=\
  $(UNITY_ROOT)/src/unity.c \
  $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
  $(CRYPTO_ROOT)/src/crypt.c \
  $(CRYPTO_ROOT)/src/xtea.c \
  $(CRYPTO_ROOT)/src/aes.c \
  $(CRYPTO_ROOT)/src/blowfish.c \
  $(CRYPTO_ROOT)/tests/test_crypto.c \
  $(CRYPTO_ROOT)/tests/test_runners/test_crypto_runner.c \
  $(CRYPTO_ROOT)/tests/test_runners/all_tests.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src -I$(UNITY_ROOT)/extras/fixture/src

run_unity: 
	gcc $(CFLAGS) $(INC_DIRS) $(SRC_FILES1) -o $(TARGET1)
	- ./$(TARGET1) -v
	rm -fr all_tests

run_all:
	make run_unity
	make run_gcov
	make run_valgrind
	make run_sanitizer
	make run_code $(RUN_ALL_ARGS)
