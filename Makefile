# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/03 22:51:05 by scavalli          #+#    #+#              #
#    Updated: 2026/05/07 12:43:57 by scavalli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILE =  src/main.c src/utils.c src/thread_process.c
NAME = philo
CC = clang
CFLAGS = -Wall -Wextra -Werror -g3
SRCS = $(FILE)
OBJS = ${SRCS:.c=.o}

.PHONY: all clean fclean re

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all