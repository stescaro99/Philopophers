/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:42:01 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/11 14:06:12 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	*check_death(void *arg)
{
	t_philo		*philo;
	t_data		*d;

	philo = (t_philo *)arg;
	d = philo->data;
	usleep(d->die_time * 1000 + 8500);
	sem_wait(philo->meal_time_sem);
	if (get_time(d->start_time) - philo->meal_time >= d->die_time)
	{
		ft_print_status(philo, "\033[31mdied");
		sem_post(philo->meal_time_sem);
		sem_wait(d->print_sem);
		philo->stop = 1;
		philo->data->game_over_sem = sem_open("/game_over", O_CREAT, 0644, 1);
		sem_post(d->print_sem);
	}
	else
		sem_post(philo->meal_time_sem);
	return (NULL);
}

static void	philo_eat(t_philo *philo)
{
	sem_wait(philo->data->forks_sem);
	ft_print_status(philo, "\033[33mhas taken a fork");
	sem_wait(philo->data->forks_sem);
	ft_print_status(philo, "\033[33mhas taken a fork");
	sem_wait(philo->meal_time_sem);
	philo->meals_eaten++;
	pthread_create(&philo->tid, NULL, &check_death, philo);
	pthread_detach(philo->tid);
	ft_print_status(philo, "\033[32mis eating");
	philo->meal_time = get_time(philo->data->start_time);
	sem_post(philo->meal_time_sem);
	usleep(philo->data->eat_time * 1000);
	if (philo->meals_eaten >= philo->data->meals)
	{
		sem_wait(philo->data->print_sem);
		philo->stop = 2;
		sem_post(philo->data->print_sem);
	}
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
	ft_print_status(philo, "\033[34mis sleeping");
	usleep(philo->data->slp_time * 1000);
	ft_print_status(philo, "\033[35mis thinking");
}

static void	philos(t_philo *philo)
{
	short	ret;

	pthread_create(&philo->tid, NULL, &check_death, philo);
	pthread_detach(philo->tid);
	if (philo->id % 2 == 0)
		ft_print_status(philo, "\033[35mis thinking");
	if (philo->id % 2 == 0)
		usleep(philo->data->eat_time * 1000);
	while (1)
	{
		sem_wait(philo->data->print_sem);
		if (philo->stop != 0)
		{
			sem_post(philo->data->print_sem);
			break ;
		}
		sem_post(philo->data->print_sem);
		philo_eat(philo);
	}
	sem_wait(philo->data->print_sem);
	ret = philo->stop;
	sem_post(philo->data->print_sem);
	usleep((philo->data->die_time + philo->data->eat_time) * 1000 + 9999);
	ft_free_all(philo->data);
	exit(ret);
}

short	philosophers(t_philo **table, t_data d)
{
	size_t		i;
	t_philo		*philo;

	philo = *table;
	i = -1;
	while (++i < d.n_phil)
	{
		philo->pid = fork();
		if (philo->pid == -1)
			return (write(2, "Error: failed to create process\n", 33) - 34);
		if (philo->pid == 0)
			philos(philo);
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
