/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 16:02:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/31 16:02:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# include <sys/stat.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <termcap.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>
# include <signal.h>
# include <sys/syslimits.h>

# include "execution.h"

/*
** Character sent by terminal line discipline in response to RETURN key
*/

# define RETURN 0x0a

/*
** Character sent by terminal line discipline in response to BACKSPACE key
*/

# define DEL 0x7f

/*
** Character sent by terminal line discipline in response to ctrl-c
*/

# define INTR 0x03

/*
** Character sent by terminal line discipline in response to ctrl-d
*/

# define EOT 0x04

/*
** Used in lexical analysis to when unclosed quoted
** strings, command substitutions detected
*/

t_list	*g_missing;

/*
<<<<<<< HEAD
** Stores the {name=val} pairs of environment variables for the shell
=======
** Character sent by terminal line discipline in response to DELETE key
*/

# define DEL2 0x7E335B1B

/*
** Character sent by terminal line discipline in response to UP key
*/

# define UP 0x415B1B

/*
** Character sent by terminal line discipline in response to CTL_UP key
*/

# define CTL_UP 0x41353B315B1B

/*
** Character sent by terminal line discipline in response to DOWN key
*/

# define DOWN 0x425B1B

/*
** Character sent by terminal line discipline in response to CTL_DOWN key
*/

# define CTL_DOWN 0x42353B315B1B

/*
** Character sent by terminal line discipline in response to RIGHT key
*/

# define RIGHT 0x435B1B

/*
** Character sent by terminal line discipline in response to CTL_RIGHT key
*/

# define CTL_RIGHT 0x43353B315B1B

/*
** Character sent by terminal line discipline in response to LEFT key
*/

# define LEFT 0x445B1B

/*
** Character sent by terminal line discipline in response to CTL_LEFT key
*/

# define CTL_LEFT 0x44353B315B1B

/*
** Character sent by terminal line discipline in response to HOME key
*/

# define HOME 0x485B1B

/*
** Character sent by terminal line discipline in response to END key
*/

# define END 0x465B1B

/*
** Character sent by terminal line discipline in response to ctrl-c
>>>>>>> move_words
*/

char	**g_environ;

/*
** Track # of current processes, used by signal handlers to determine whether
** shell should exit current child process or catch exit
** signal of parent process
*/

int		g_processes;

/*
** src/init/init.c
*/

<<<<<<< HEAD
int		init_environ(int argc, char **argv, char **environ);
int		init_parser(void);
int		init_shell();
int		restore_shell(void);
void	free_prods(void);
=======
int				init_environ(int argc, char **argv, char **environ);
int				init_parser(void);
int				init_shell(t_interface *interface);
int				restore_shell(t_interface *interface);
>>>>>>> move_words

/*
** src/init/signal.c
*/

void	sigtrap_handler(int sig);
void	child_sig_handler(int sig);
void	sig_handler(int sig);

/*
** src/init/terminal_modes.c
*/

<<<<<<< HEAD
int		prep_terminal(struct termios *ttys, int flags);
int		restore_terminal(struct termios *tty);
int		ft_termprint(int c);
=======
int				prep_terminal(struct termios *ttys, int flags, int vmin, int vtime);
int				restore_terminal(struct termios *tty);
int				ft_termprint(int c);
>>>>>>> move_words

/*
** builtins/
*/

int		builtin_echo(int argc, char **argv);
int		builtin_cd(int argc, char **argv);
int		builtin_setenv(int argc, char **argv);
int		builtin_unsetenv(int argc, char **argv);
int		builtin_exec(int argc, char **argv);
int		builtin_env(int argc, char **argv);
void	set_env_var(char *name, char *val);
char	*get_env_var(char *var);

/*
** src/utils/utils.c
*/

int		escaped(char *input, size_t i);
void	free_prods(void);
char	**push_pointer_back(char **pointers, t_ast_node *node);
void	**push_pointer_front(void **pointers, void *ptr);
#endif
