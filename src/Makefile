CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -g

CHECK_LIBS = -lcheck -lsubunit -lpthread -lm
GCOV=

SRC_DIR = .
OBJ_DIR = object_files
TEST_DIR = tests

DECIMAL_SRCS=$(wildcard $(SRC_DIR)/s21*.c) $(SRC_DIR)/utils.c
DECIMAL_HEADER=$(wildcard $(SRC_DIR)/*.h)

TEST_SRCS=$(wildcard $(TEST_DIR)/*.c)
TEST_HEADERS=$(wildcard $(TEST_DIR)/*.h)

DECIMAL_OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(DECIMAL_SRCS))
TEST_OBJS=$(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRCS))

LIB=s21_decimal.a
TEST_EXEC=$(TEST_DIR)/test

all: gcov_report

$(TEST_EXEC): $(LIB) $(TEST_OBJS)
	@$(CC) $(TEST_OBJS) $(LIB) -o $@ $(CHECK_LIBS) $(GCOV)

$(OBJ_DIR):
	@mkdir $@ 
# -p

$(LIB): $(OBJ_DIR) $(DECIMAL_OBJS)
	@ar rcs $@ $(DECIMAL_OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DECIMAL_HEADER) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(TEST_HEADERS) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@


test: clean $(TEST_EXEC)
	@./$(TEST_EXEC)


gcov_report: CFLAGS+= -fprofile-arcs -ftest-coverage
gcov_report: GCOV+= -lgcov
gcov_report: clean test
	lcov  --capture --directory . --output-file $(TEST_DIR)/coverage.info
	mkdir -p report
	genhtml $(TEST_DIR)/coverage.info --output-directory report
	

clean:
	@rm -rf $(OBJ_DIR) $(LIB) $(TEST_EXEC) $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcov $(TEST_DIR)/coverage.info report

format:
    #@cp ../materials/linters/.clang-format .clang-format
	@clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h #$(TEST_DIR)/*.c $(TEST_DIR)/*.h
    #@rm -f .clang-format

memory_check: clean $(TEST_EXEC)
	@valgrind --leak-check=full --quiet $(TEST_EXEC)
# --errors-for-leak-kinds=all

cppcheck:
	@cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR)*.c $(TEST_DIR)/*.c

show_coverage_info: clean gcov_report
	@yandex-browser report/index.html

.PHONY: all clean test gcov_report format memory_check cppcheck show_coverage_info
