/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:42:10 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/04 10:21:40 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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

uint64_t	get_time(uint64_t start)
{
	struct timeval	times;

	gettimeofday(&times, NULL);
	return ((uint64_t)((times.tv_sec * 1000 + times.tv_usec / 1000) - start));
}

void	ft_print_status(t_philo *philo, char *str)
{
	sem_wait(philo->data->print_sem);
	if (philo->stop == 0)
	{
		philo->data->game_over_sem = sem_open("/game_over", 0);
		if (philo->data->game_over_sem == SEM_FAILED)
		{
			printf("\033[0m%lu %lu %s\033[38;5;208m\n",
				get_time(philo->data->start_time), philo->id, str);
		}
		else if (philo->data->game_over_sem != SEM_FAILED)
		{
			philo->stop = 1;
			sem_close(philo->data->game_over_sem);
		}
	}
	else
		sem_close(philo->data->game_over_sem);
	sem_post(philo->data->print_sem);
}
