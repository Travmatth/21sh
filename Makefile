DEBUG =
ifdef TEST
        DEBUG = -g -fsanitize=address -fsanitize=undefined
else
        DEBUG =
endif
ifdef LEAKS
        DEBUG = -g
endif

NAME = 21sh
LIBFT = libftprintf/libftprintf.a
CFLAGS += -Wall -Wextra -Werror -Wpedantic
LDFLAGS := -Llibftprintf -lftprintf -I./includes -ltermcap
CORE := main
BUILTINS := cd echo env setenv unsetenv exec 
LEXICAL := lexer lexer_rules_1 lexer_rules_2 lexer_utils operator_dfa reserved_dfa process_token
SYNTACTIC := parse parse_stack_utils parse_table parse_utils
SEMANTIC := affixes command pipe semantic separator redir_utils verify_command
EXPANSIONS := arithmetic_expansion command_substitution expansion expansion_subtypes field_splitting parameter_expansion parameter_expansion_actions parameter_expansion_utils pathname_expansion quote_removal tilde_expansion
EXECUTION := execute here_end here_end_utils operators redirs
UTILS := signal init quote_management 
FILES := $(addprefix src/, $(CORE)) \
		$(addprefix src/builtins/, $(BUILTINS)) \
		$(addprefix src/utils/, $(UTILS)) \
		$(addprefix src/lexical_analysis/, $(LEXICAL)) \
		$(addprefix src/syntactic_analysis/, $(SYNTACTIC)) \
		$(addprefix src/semantic_analysis/, $(SEMANTIC)) \
		$(addprefix src/semantic_analysis/expansions/, $(EXPANSIONS)) \
		$(addprefix src/execution/, $(EXECUTION))
SRC := $(addsuffix .c, $(FILES))
OBJ := $(SRC:.c=.o)

.PHONY = all clean fclean re

all: $(SUBMODULES) $(NAME)

$(SUBMODULES):
	@git submodule init
	@git submodule update

$(LIBFT):
	@$(MAKE) -C libftprintf

$(OBJ): %.o: %.c
	@$(CC) -c $(DEBUG) -I. $(CFLAGS) $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	@echo -n 'Compiling 21sh... '
	@$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@
	@echo "\033[32mdone\033[0m"

clean:
	@$(MAKE) clean -C libftprintf
	@echo -n 'Cleaning 21sh object files... '
	@rm -f $(OBJ) *.dSYM *.DS_Store
	@echo "\033[32mdone\033[0m"

fclean: clean
	@$(MAKE) fclean -C libftprintf
	@echo -n 'Cleaning 21sh executable... '
	@rm -f $(NAME)
	@echo "\033[32mdone\033[0m"

re: fclean all

