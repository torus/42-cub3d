/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 15:39:46 by thisai            #+#    #+#             */
/*   Updated: 2021/03/12 15:39:46 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "cub3d.h"
#include "cub3d_int.h"

void	c3_xy_queue_enqueue(
	t_c3_state *stat, t_c3_xy_queue *q, int x, int y)
{
	t_c3_xy_list	*head;

	head = malloc(sizeof(t_c3_xy_list));
	if (!head)
	{
		C3_LOG("Error\nCouldn't allocate memory.\n");
		c3_terminate(stat);
		exit(1);
	}
	head->x = x;
	head->y = y;
	head->next = q->first;
	head->prev = NULL;
	if (q->first)
		q->first->prev = head;
	q->first = head;
	if (!q->last)
		q->last = head;
	return ;
}

int		c3_xy_queue_dequeue(t_c3_xy_queue *q, int *x, int *y)
{
	t_c3_xy_list	*last;

	last = q->last;
	if (!last)
		return (0);
	*x = last->x;
	*y = last->y;
	q->last = last->prev;
	if (!q->last)
		q->first = NULL;
	if (last->prev)
		last->prev->next = NULL;
	free(last);
	return (1);
}

void	c3_check_map_error(t_c3_state *stat, t_c3_xy_queue *q)
{
	t_c3_xy_list	*l;
	t_c3_xy_list	*tmp;

	l = q->first;
	while (l)
	{
		tmp = l->next;
		free(l);
		l = tmp;
	}
	C3_LOG("Error\nThe map is not closed.\n");
	c3_terminate(stat);
	exit(1);
}

void	c3_check_map_closed_iter(t_c3_state *stat, int x, int y)
{
	char			ch;
	t_c3_xy_queue	q;

	q.first = NULL;
	q.last = NULL;
	c3_xy_queue_enqueue(stat, &q, x, y);
	while (c3_xy_queue_dequeue(&q, &x, &y))
		if (x >= 0 && x < stat->map.width && y >= 0 && y < stat->map.height)
		{
			ch = stat->map.map[y * stat->map.width + x];
			if (ch != '1' && ch != 'x' && ch != 'o')
			{
				if (ch == '2')
					stat->map.map[y * stat->map.width + x] = 'o';
				else
					stat->map.map[y * stat->map.width + x] = 'x';
				c3_xy_queue_enqueue(stat, &q, x, y - 1);
				c3_xy_queue_enqueue(stat, &q, x + 1, y);
				c3_xy_queue_enqueue(stat, &q, x, y + 1);
				c3_xy_queue_enqueue(stat, &q, x - 1, y);
			}
		}
		else
			c3_check_map_error(stat, &q);
}
