NAME = libcsv.so
TEST_NAME = unitTest

SRC = libcsv.c helper.c
TEST_SRC = unitTest.c $(SRC)

INCLUDE = libcsv.h helper.h
TEST_INCLUDE = unitTest.h $(INCLUDE)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

all: $(NAME)

$(NAME): $(SRC) $(INCLUDE)
	$(CC) $(CFLAGS) -shared -o $(NAME) $(SRC)

test: clean $(TEST_NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_NAME)
	$(RM) $(TEST_NAME)
	$(CC) $(CFLAGS) -fsanitize=address -o $(TEST_NAME) $(TEST_SRC)
	./$(TEST_NAME)
	$(RM) $(TEST_NAME)

$(TEST_NAME): $(NAME) $(TEST_SRC) $(INCLUDE)
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_SRC) $(NAME) -Wl, -rpath =$(pwd)

clean:
	$(RM) $(TEST_NAME)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re test