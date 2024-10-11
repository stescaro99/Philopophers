/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:14:18 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/04 10:15:39 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

short	game_over(t_data *d)
{
	pthread_mutex_lock(&d->game_over_mutex);
	if (d->game_over == 1)
	{
		pthread_mutex_unlock(&d->game_over_mutex);
		return (1);
	}
	pthread_mutex_unlock(&d->game_over_mutex);
	return (0);
}

void	set_game_over(t_data *d)
{
	pthread_mutex_lock(&d->game_over_mutex);
	if (d->game_over == 0)
		d->game_over = 1;
	pthread_mutex_unlock(&d->game_over_mutex);
}

void	ft_print_status(t_data *d, size_t id, char *str)
{
	pthread_mutex_lock(&d->game_over_mutex);
	if (d->game_over != 1)
	{
		pthread_mutex_unlock(&d->game_over_mutex);
		pthread_mutex_lock(&d->print_mutex);
		printf("\033[0m%lu %lu %s\033[38;5;208m\n",
			get_time(d->start_time), id, str);
		pthread_mutex_unlock(&d->print_mutex);
	}
	else
		pthread_mutex_unlock(&d->game_over_mutex);
}

size_t	ft_atos(char *str)
{
	size_t	n;
	size_t	i;

	i = 0;
	n = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		n = n * 10 + (str[i] - 48);
		i++;
	}
	if (str[i] != 0)
		return (0);
	return (n);
}

size_t	get_time(size_t start)
{
	struct timeval	times;

	gettimeofday(&times, NULL);
	return ((size_t)((times.tv_sec * 1000 + times.tv_usec / 1000) - start));
}
