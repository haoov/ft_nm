#------------------------------------#
#               Project              #
#------------------------------------#
TARGET		=	ft_nm
PROJDIR		=	$(realpath $(CURDIR))
SRCDIR		=	$(PROJDIR)/srcs
INCDIR		=	$(PROJDIR)/incs
OBJDIR		=	$(PROJDIR)/objs
DEPDIR		=	$(PROJDIR)/.deps

#------------------------------------#
#               Compiler             #
#------------------------------------#
CC			=	gcc
CFLAGS		=	-g
INCLUDE		=	-I $(INCDIR)

#------------------------------------#
#                Files               #
#------------------------------------#
SRCS		=	$(shell find $(SRCDIR) -type f -name '*'.c)
OBJS		=	$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCS:.c=.o))
DEPS		=	$(patsubst $(SRCDIR)/%,$(DEPDIR)/%,$(SRCS:.c=.d))
LIBFT		=	$(PROJDIR)/libft/libft.a

#------------------------------------#
#                Rules               #
#------------------------------------#
all : $(TARGET)

$(TARGET) : $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBFT) -o $@
	@echo Linking $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@mkdir -p $(patsubst $(OBJDIR)/%,$(DEPDIR)/%,$(dir $@))
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -MMD -MF $(patsubst $(OBJDIR)/%,$(DEPDIR)/%,$(@:.o=.d))
	@echo Building $(notdir $@)

-include $(DEPS)

$(LIBFT) :
	@echo Building libft
	@make -s -C $(PROJDIR)/libft

clean :
	@rm -rf $(OBJDIR)
	@rm -rf $(DEPDIR)
	@echo Cleaning object and dependency files
	@make -C $(PROJDIR)/libft clean

fclean : clean
	@make -C $(PROJDIR)/libft fclean
	@rm -f $(TARGET)
	@echo Cleaning $(TARGET)

re : fclean all

.PHONY : all clean fclean re