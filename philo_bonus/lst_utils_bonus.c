/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:41:35 by stescaro          #+#    #+#             */
/*   Updated: 2024/02/29 10:43:15 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

t_philo	*ft_lstnew(size_t i, t_data *d)
{
	t_philo		*new_philo;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	new_philo->id = i;
	new_philo->meals_eaten = 0;
	new_philo->stop = 0;
	new_philo->pid = 0;
	new_philo->tid = 0;
	new_philo->meal_time = 0;
	new_philo->data = d;
	new_philo->meal_time_sem = sem_open("/meal_time", O_CREAT, 0644, 1);
	new_philo->next = NULL;
	new_philo->prev = NULL;
	return (new_philo);
}

void	ft_lstclear(t_philo **table, t_data d)
{
	uint32_t	i;
	t_philo		*philo;
	t_philo		*prev;

	philo = *table;
	i = -1;
	while (++i < d.n_phil)
	{
		prev = philo;
		philo = philo->next;
		sem_close(prev->meal_time_sem);
		sem_unlink("/meal_time");
		free(prev);
	}
	free(table);
}

int	ft_free_all(t_data *data)
{
	sem_close(data->forks_sem);
	sem_wait(data->print_sem);
	sem_close(data->game_over_sem);
	sem_post(data->print_sem);
	sem_close(data->print_sem);
	if (data->table)
		ft_lstclear(data->table, *data);
	free(data);
	return (1);
}
