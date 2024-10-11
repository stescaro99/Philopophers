NAME        := philo_bonus
CC        	:= cc
FLAGS    	:= -Wall -Wextra -Werror
SRCS        := main_bonus.c philosophers_bonus.c utils_bonus.c lst_utils_bonus.c                       
OBJS        := $(SRCS:.c=.o)

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

${NAME}:	${OBJS}
			echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${FLAGS} -pthread -o ${NAME} ${OBJS}
			clear
			@echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}

bonus:		all

clean:
			${RM} *.o */*.o */*/*.o
			clear
			@echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			${RM} ${NAME}
			clear
			@echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

fast:		all clean
			clear
			@echo "$(GREEN)$(NAME) created[0m ✔️"
			
eval:		fast
			code .
			clear
			norminette			

.PHONY:		all clean fclean re fast push eval
