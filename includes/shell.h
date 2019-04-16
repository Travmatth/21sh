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
** Signature of shell builtin functions
*/

typedef int		(*t_builtinf)(int argc, char **argv);

/*
** Structs used to match name of builtin to its function pointer
*/

typedef struct	s_builtin
{
	char		*cmd;
	t_builtinf	f;
	int			len;
}				t_builtin;

/*
** Global variable holding structs mapping builtin names to function pointers
*/

extern t_builtin	g_builtins[];

/*
** src/main.c
*/

/*
** src/init.c
*/

void			init_environ(int argc, char **argv, char **environ);
void			init_parser(void);
int				init_shell(void);

/*
** signal.c
*/

void			child_sig_handler(int sig);
void			sig_handler(int sig);

/*
** builtins/
*/

void			set_env_var(char *name, char *val);
int				builtin_echo(int argc, char **argv);
int				builtin_cd(int argc, char **argv);
int				builtin_setenv(int argc, char **argv);
int				builtin_unsetenv(int argc, char **argv);
int				builtin_env(int argc, char **argv);
char			*get_env_var(char *var);
#endif
