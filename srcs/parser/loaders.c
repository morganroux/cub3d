/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroux <mroux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 11:02:45 by mroux             #+#    #+#             */
/*   Updated: 2020/03/09 16:27:41 by mroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		load_map_dimensions(t_game_engine *ge, char *line)
{
	char	**param;
	int		i;

	param = ft_split(line, ' ');
	if (check_dim_param(param) != OK)
		return (ERROR_DIM);
	ge->screen_w = ft_atoi(param[1]);
	ge->screen_w = ge->screen_w > 2560 ? 2560 : ge->screen_w;
	ge->screen_h = ft_atoi(param[2]);
	ge->screen_h = ge->screen_h > 1440 ? 1440 : ge->screen_h;
	i = 0;
	while (param[i] != 0)
		free(param[i++]);
	free(param);
	if (ge->screen_h == 0 || ge->screen_w == 0)
		return (ERROR_DIM);
	return (OK);
}

int		load_colors(t_game_engine *ge, char *line)
{
	char	**param;
	int		n;
	int		i;

	param = ft_split(&(line[2]), ',');
	if (check_color_param(param) == ERROR)
		return (ERROR);
	n = (line[0] == 'F') ? 0 : 1;
	ge->map.color[n].r = ft_atoi(param[0]);
	ge->map.color[n].v = ft_atoi(param[1]);
	ge->map.color[n].b = ft_atoi(param[2]);
	i = 0;
	while (param[i] != 0)
		free(param[i++]);
	free(param);
	return (OK);
}

int		load_sprite(t_game_engine *ge, char *line)
{
	t_sprite	*sprite;
	char		**param;
	int			i;
	
	i = 0;
	param = ft_split(line, ' ');
	if (check_sprite_param(param) == ERROR)
		return (ERROR);
	while (i < SPRITE_NUMBER)
	{
		sprite = &ge->map.sprite[i];
		sprite->texture.path = param[1];
		sprite->texture.p_img = mlx_xpm_file_to_image(
						ge->mlx_ptr,
						sprite->texture.path,
						&sprite->texture.w, &sprite->texture.h);
		if (sprite->texture.p_img == 0)
			return (ERROR);
		sprite->texture.data = mlx_get_data_addr(
					sprite->texture.p_img, &sprite->texture.bits_per_pxl,
					&sprite->texture.size_line, &sprite->texture.endian);
		i++;
	}
	i = 0;
	while (param[i] != 0)
		free(param[i++]);
	free(param);
	return (OK);
}

int		load_map(t_game_engine *ge, int fd, char *firstline)
{
	int		n;
	char	*line;

	n = 0;
	ge->map.w = ft_strlen(firstline);
	ge->map.h = 0;
	ge->map.p_map = NULL;
	load_line(ge, firstline, &n);
	while (get_next_line(fd, &line))
	{
		load_line(ge, line, &n);
		free(line);
	}
	load_line(ge, line, &n);
	ge->map.size = n;
	free(line);
	check_map(ge);
	return (OK);
}

int		load_textures(t_game_engine *ge, char *line)
{
	t_img	*tex;
	char	**param;
	int		i;

	param = ft_split(line, ' ');
	if (check_tex_param(param) == ERROR)
		return (ERROR);
	tex = &ge->map.textures[get_tex_orientation(line[0])];
	tex->path = param[1];
	tex->p_img = mlx_xpm_file_to_image(
					ge->mlx_ptr,
					tex->path,
					&tex->w, &tex->h);
	if (tex->p_img == 0)
		return (ERROR);
	tex->data = mlx_get_data_addr(
				tex->p_img, &tex->bits_per_pxl,
				&tex->size_line, &tex->endian);
	i = 0;
	while (param[i] != 0)
		free(param[i++]);
	free(param);
	return (OK);
}
