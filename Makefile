NAME = libcsv.so
TEST_NAME = unitTest.out

SRC = libcsv.c
TEST_SRC = unitTest.c $(SRC)

INCLUDE = libcsv.h
TEST_INCLUDE = unitTest.h $(INCLUDE)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

all: test

test: clean $(TEST_NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_NAME)

sanitize: clean
	$(CC) $(CFLAGS) -fsanitize=address -o $(TEST_NAME) $(TEST_SRC)
	./$(TEST_NAME)


$(TEST_NAME): $(TEST_SRC) $(INCLUDE)
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_SRC)

clean:
	$(RM) $(TEST_NAME)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re test