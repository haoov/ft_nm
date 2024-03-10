#------------------------------------#
#               Project              #
#------------------------------------#
TARGET		=	libft.a
PROJDIR		=	$(realpath $(CURDIR))
SRCDIR		=	$(PROJDIR)/srcs
INCDIR		=	$(PROJDIR)/incs
OBJDIR		=	$(PROJDIR)/objs
DEPDIR		=	$(PROJDIR)/.deps

#------------------------------------#
#               Compiler             #
#------------------------------------#
CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror
INCLUDE		=	-I $(INCDIR)

#------------------------------------#
#                Files               #
#------------------------------------#
SRCS		=	$(shell find $(SRCDIR) -type f -name '*'.c)
OBJS		=	$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCS:.c=.o))
DEPS		=	$(patsubst $(SRCDIR)/%,$(DEPDIR)/%,$(SRCS:.c=.d))

#------------------------------------#
#                Rules               #
#------------------------------------#
all : $(TARGET)

$(TARGET) : $(OBJS)
	@ar -rcs $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@mkdir -p $(patsubst $(OBJDIR)/%,$(DEPDIR)/%,$(dir $@))
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -MMD -MF $(patsubst $(OBJDIR)/%,$(DEPDIR)/%,$(@:.o=.d))

-include $(DEPS)

clean :
	@rm -rf $(OBJDIR)
	@rm -rf $(DEPDIR)
	@echo Cleaning object and dependency files

fclean : clean
	@rm -f $(TARGET)
	@echo Cleaning $(TARGET)

re : fclean all

.PHONY : all clean fclean re