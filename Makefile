NAME = minishell

#COLORS
BLUE = \033[1;34m
GREY = \033[8;90m
GREEN = \033[1;36m
RED = \033[1;31m
END = \033[0m

#INCLUDE
INCLUDEPATH = includes
LIBFTPATH = libft

INCLUDES = $(INCLUDEPATH)/*.h

#LIB
LIBFT = $(LIBFTPATH)/libft.a

#FLAG
CC = clang
FLAGS = -Wall -Wextra -Werror

#SRC & OBJ PATH
SRCPATH = ./srcs
OBJPATH = $(SRCPATH)/obj

#SRCS --- *.c is temporary but I think usefull at dvpmt time
SRCS = $(addprefix $(SRCPATH)/, *.c)

#OBJS
OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)



#RULES

all : mk_objdir mk_libft $(NAME)



mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi

mk_libft :
	@echo "\n$(END)$(BLUE)Checking libft$(END)$(GREY)"
	@make -C $(LIBFTPATH)



$(NAME) : message $(OBJ) $(INCLUDES)
	@echo "\n$(END)$(BLUE)Making $(NAME)$(END)$(GREY)"
	$(CC) -o $@ $(OBJ) $(LIBFT)
	@echo "\n$(END)$(GREEN)$(NAME) is built$(END)"

message :
	@echo "\n$(END)$(BLUE)Making $(NAME) objects$(END)$(GREY)"



$(OBJPATH)/%.o : $(SRCPATH)/%.c $(INCLUDES)
	$(CC) $(FLAGS) -I $(LIBFTPATH) -I $(INCLUDEPATH) -c $< -o $@



#CLEAN
.PHONY : clean fclean re

clean :
	@echo "$(END)$(RED)\nremoving $(NAME) objects$(END)$(GREY)"
	rm -rf $(OBJ)
	@echo "$(END)$(RED)cleaning libft objects$(END)"
	@make clean -C $(LIBFTPATH)

fclean : clean
	@echo "$(END)$(RED)\nremoving $(NAME)$(END)$(GREY)"
	@rm -f $(NAME)
	@echo "$(END)$(RED)\nremoving libft.a$(END)"
	@make fclean -C $(LIBFTPATH)

re : fclean all
