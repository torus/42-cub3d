/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_color2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 15:28:19 by thisai            #+#    #+#             */
/*   Updated: 2021/02/26 15:28:19 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"

int					c3_scene_check_colors_specified(t_c3_scene_parser *buf)
{
	if (buf->is_specified[C3_SCENE_TOKEN_C]
		&& buf->is_specified[C3_SCENE_TOKEN_F])
		return (1);
	return (0);
}
