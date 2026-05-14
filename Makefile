NAME = irc

CXX = c++
CXXFLAGS = -Werror -Wextra -Wall -std=c++98

SRCS = srcs/main.cpp
OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I includes

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

srcs/main.o: includes/irc.hpp

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
