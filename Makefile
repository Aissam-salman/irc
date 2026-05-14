NAME = irc

CXX = c++
CXXFLAGS = -Werror -Wextra -Wall -std=c++98

SRCS = srcs/main.cpp

OBJ_DIR = objs

OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

INCLUDES = -I includes


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all


leaks: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
           --suppressions=readline.supp --track-origins=yes ./$(NAME)


.PHONY: all clean fclean re leaks
