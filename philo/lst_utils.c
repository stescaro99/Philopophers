/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:05:24 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/13 10:16:21 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo	*ft_lstnew(size_t n, t_data *data)
{
	t_philo		*new_philo;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	new_philo->id = n;
	new_philo->eaten = 0;
	new_philo->thread_id = 0;
	new_philo->thread2_id = 0;
	new_philo->meal_time = 0;
	new_philo->data = data;
	pthread_mutex_init(&new_philo->fork_mutex, NULL);
	pthread_mutex_init(&new_philo->eat_mutex, NULL);
	new_philo->next = NULL;
	new_philo->prev = NULL;
	return (new_philo);
}

void	ft_lstclear(t_philo **table, size_t num_philo)
{
	size_t		i;
	t_philo		*philo;
	t_philo		*prev;

	philo = *table;
	i = 0;
	while (i < num_philo && philo)
	{
		prev = philo;
		philo = philo->next;
		if (prev->data->satisfied != prev->data->n_phil)
		{
			pthread_mutex_unlock(&prev->fork_mutex);
			pthread_mutex_unlock(&prev->eat_mutex);
		}
		pthread_mutex_destroy(&prev->fork_mutex);
		pthread_mutex_destroy(&prev->eat_mutex);
		free(prev);
		i++;
	}
	free(table);
}

short	ft_free_all(t_data *data, t_philo **table)
{
	if (!table || !(*table))
		return (1);
	if (data->satisfied != data->n_phil)
	{
		pthread_mutex_unlock(&data->game_over_mutex);
		pthread_mutex_unlock(&data->satisfied_mutex);
		pthread_mutex_unlock(&data->print_mutex);
	}
	pthread_mutex_destroy(&data->game_over_mutex);
	ft_lstclear(table, data->n_phil);
	pthread_mutex_destroy(&data->satisfied_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	free(data);
	return (1);
}
