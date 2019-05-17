/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:06:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/29 18:04:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

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
# include "interface.h"

/*
** Used in lexical analysis to when unclosed quoted
** strings, command substitutions detected
*/

t_list		*g_missing;

/*
** Stores the {name=val} pairs of environment variables for the shell
*/

char			**g_environ;

/*
** Track # of current processes, used by signal handlers to determine whether
** shell should exit current child process or catch exit
** signal of parent process
*/

int				g_processes;

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
** src/main.c
*/

/*
** src/init/init.c
*/

int				init_environ(int argc, char **argv, char **environ);
int				init_parser(void);
int				init_shell();
int				restore_shell(void);
void			free_prods(void);

/*
** src/init/signal.c
*/

void			sigtrap_handler(int sig);
void			child_sig_handler(int sig);
void			sig_handler(int sig);

/*
** src/init/terminal_modes.c
*/

int				prep_terminal(struct termios *ttys, int flags);
int				restore_terminal(struct termios *tty);
int				ft_termprint(int c);

/*
** builtins/
*/

int				builtin_echo(int argc, char **argv);
int				builtin_cd(int argc, char **argv);
int				builtin_setenv(int argc, char **argv);
int				builtin_unsetenv(int argc, char **argv);
int				builtin_exec(int argc, char **argv);
int				builtin_env(int argc, char **argv);
void			set_env_var(char *name, char *val);
char			*get_env_var(char *var);
#endif
