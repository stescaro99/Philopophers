/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:31:20 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/04 13:46:26 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static	void	*checker(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	usleep(data->die_time * 1000 + 8500);
	pthread_mutex_lock(&philo->eat_mutex);
	if (get_time(data->start_time) - philo->meal_time >= data->die_time
		&& !game_over(data))
	{
		printf("\033[0m%lu %lu \033[31mdied\033[0m\n",
			get_time(data->start_time), philo->id);
		pthread_mutex_unlock(&philo->eat_mutex);
		set_game_over(data);
	}
	else
		pthread_mutex_unlock(&philo->eat_mutex);
	return (NULL);
}

static	void	eat(t_philo *philo)
{
	t_data	*d;

	d = philo->data;
	pthread_mutex_lock(&philo->eat_mutex);
	philo->eaten++;
	ft_print_status(philo->data, philo->id, "\033[32mis eating");
	philo->meal_time = get_time(philo->data->start_time);
	pthread_mutex_unlock(&philo->eat_mutex);
	pthread_create(&philo->thread2_id, NULL, &checker, philo);
	pthread_detach(philo->thread2_id);
	usleep(philo->data->eat_time * 1000);
	if (philo->eaten >= d->meals)
	{
		pthread_mutex_lock(&d->satisfied_mutex);
		if (++(d->satisfied) >= d->n_phil)
			set_game_over(d);
		pthread_mutex_unlock(&d->satisfied_mutex);
	}
}

static	void	*ft_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_create(&philo->thread2_id, NULL, &checker, philo);
	pthread_detach(philo->thread2_id);
	if (philo->id % 2 == 0)
	{
		ft_print_status(philo->data, philo->id, "\033[35mis thinking");
		usleep(philo->data->eat_time * 1000);
	}
	while (!game_over(philo->data))
	{
		pthread_mutex_lock(&philo->fork_mutex);
		ft_print_status(philo->data, philo->id, "\033[33mhas taken a fork");
		pthread_mutex_lock(&philo->next->fork_mutex);
		ft_print_status(philo->data, philo->id, "\033[33mhas taken a fork");
		eat(philo);
		pthread_mutex_unlock(&philo->next->fork_mutex);
		pthread_mutex_unlock(&philo->fork_mutex);
		ft_print_status(philo->data, philo->id, "\033[34mis sleeping");
		usleep(philo->data->slp_time * 1000);
		ft_print_status(philo->data, philo->id, "\033[35mis thinking");
	}
	return (NULL);
}

short	philosophers(t_data *data, t_philo **table)
{
	size_t		i;
	t_philo		*philo;

	philo = *table;
	i = -1;
	while (++i < data->n_phil)
	{
		if (pthread_create(&philo->thread_id, NULL, &ft_philo, philo) == -1)
			return (write(2, "Error!\nThread creation failed\n", 31));
		philo = philo->next;
	}
	return (0);
}

t_data	*only_one(t_data *data, size_t die_time)
{
	usleep(die_time * 1000);
	printf("\033[0m%lu 1 \033[31mdied\033[0m\n", die_time);
	free(data);
	return (NULL);
}
