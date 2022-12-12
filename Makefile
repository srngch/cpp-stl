# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sarchoi <sarchoi@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/09 16:19:57 by sarchoi           #+#    #+#              #
#    Updated: 2022/12/09 16:49:16 by sarchoi          ###   ########seoul.kr   #
#                                                                              #
# **************************************************************************** #

NAME = ft_containers

CC = c++
INCLUDE_FLAGS = -I includes
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 $(INCLUDE_FLAGS)

# SRCS_FILES = stack.cpp
SRCS = main.cpp
# $(addprefix ./srcs/, $(SRCS_FILES))
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(NAME)

.PHONY: clean
clean:
	rm -f $(OBJS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all
