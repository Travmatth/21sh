/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 20:08:57 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/12 18:14:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static	char	*g_permissions = "----------";

/*
** find maximum widths of node names
*/

void	*get_max_width(void *final, t_list *elem, size_t i, int *stop)
{
	t_dir	*dir;
	size_t	size;

	(void)i;
	(void)stop;
	dir = (t_dir*)elem->content;
	size = LEN(dir->name, 0);
	if (!final)
		return (ft_memdup(&size, sizeof(size_t)));
	else if (*(size_t*)final < size)
	{
		free(final);
		return (ft_memdup(&size, sizeof(size_t)));
	}
	return (final);
}

/*
** format node colors
*/

char	*format_color(t_ls *ctx, char *template, t_dir *node)
{
	char	*fmt;

	fmt = NULL;
	if (S_ISLNK(node->mode))
		fmt = "{purple}";
	else if (S_ISDIR(node->mode))
		fmt = "{b_blue}";
	else if (S_ISBLK(node->mode))
		fmt = "{blue}";
	else if (S_ISCHR(node->mode))
		fmt = "{yellow}";
	else if (S_ISSOCK(node->mode))
		fmt = "{green}";
	else if (S_ISFIFO(node->mode))
		fmt = "{b_cyan}";
	else if (node->mode & S_IXUSR)
		fmt = "{red}";
	if ((!fmt || !GET_COLOR_OUT(ctx->flags))
		&& ft_count_char(template, '%') <= 2)
		return (ft_strdup(template));
	else if (!fmt || !GET_COLOR_OUT(ctx->flags))
		return (ft_strjoin(template, S_ISLNK(node->mode) ? "%s -> %s" : "%s"));
	return (ft_count_char(template, '%') <= 2
			? template_swap_first(template, fmt)
			: template_make_last(template, fmt, node));
}

/*
** format node name for printing
*/

char	*format_permissions(t_dir *file)
{
	char *out;

	out = ft_strdup(g_permissions);
	out[0] = S_ISDIR(file->mode) ? 'd' : '-';
	out[0] = S_ISLNK(file->mode) ? 'l' : out[0];
	out[0] = S_ISBLK(file->mode) ? 'b' : out[0];
	out[0] = S_ISCHR(file->mode) ? 'c' : out[0];
	out[0] = S_ISSOCK(file->mode) ? 's' : out[0];
	out[0] = S_ISFIFO(file->mode) ? 'p' : out[0];
	out[1] = file->mode & S_IRUSR ? 'r' : '-';
	out[2] = file->mode & S_IWUSR ? 'w' : '-';
	out[3] = file->mode & S_IXUSR ? 'x' : '-';
	out[3] = (file->mode & (USR_OR_STICK)) == S_ISUID ? 'S' : out[3];
	out[3] = (file->mode & (USR_OR_STICK)) == (USR_OR_STICK) ? 's' : out[3];
	out[4] = file->mode & S_IRGRP ? 'r' : '-';
	out[5] = file->mode & S_IWGRP ? 'w' : '-';
	out[6] = file->mode & S_IXGRP ? 'x' : '-';
	out[6] = (file->mode & (GRP_OR_STICK)) == S_ISGID ? 'S' : out[6];
	out[6] = (file->mode & (GRP_OR_STICK)) == (GRP_OR_STICK) ? 's' : out[6];
	out[7] = file->mode & S_IROTH ? 'r' : '-';
	out[8] = file->mode & S_IWOTH ? 'w' : '-';
	out[9] = file->mode & S_IXOTH ? 'x' : '-';
	out[9] = (file->mode & (OTH_OR_STICK)) == S_ISVTX ? 'T' : out[9];
	out[9] = (file->mode & (OTH_OR_STICK)) == (OTH_OR_STICK) ? 't' : out[9];
	return (out);
}

/*
** format node date for printing
*/

char	*format_date(char *date)
{
	char	*out;
	char	**parts;
	size_t	i;

	parts = ft_strsplit(date, ' ');
	out = ft_strnew(12);
	parts[4][4] = '\0';
	ft_memcpy((void*)out, parts[1], 3);
	out[3] = ' ';
	out[4] = parts[2][1] ? parts[2][0] : ' ';
	out[5] = parts[2][1] ? parts[2][1] : parts[2][0];
	out[6] = ' ';
	ft_memcpy((void*)(out + 7), parts[3], 5);
	i = 4;
	while (i)
		free(parts[i--]);
	free(parts[0]);
	free(parts);
	return (out);
}

/*
** format node name for printing
*/

char	*format_name(t_dir *node)
{
	char	buf[256];
	ssize_t	bytes;
	char	*name;

	if (!S_ISLNK(node->mode))
		return (ft_strdup(node->name));
	ft_bzero(buf, 256);
	name = node->full ? node->full : node->name;
	bytes = readlink(name, buf, 255);
	return (ft_strdup(buf));
}