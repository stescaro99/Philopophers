/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:41:39 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/13 10:02:30 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	wait_processes(t_data d)
{
	uint32_t	i;

	i = 0;
	while (++i <= d.n_phil)
		waitpid(-1, NULL, 0);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/game_over");
}

static t_philo	**set_table(t_data *data)
{
	size_t	i;
	t_philo	**table;
	t_philo	*new_philo;
	t_philo	*prev;

	table = malloc(sizeof(t_philo *) * data->n_phil);
	if (!table && write(2, "Error!\nTable allocation failed\n", 32))
		return (NULL);
	prev = NULL;
	i = 0;
	while (++i <= data->n_phil)
	{
		new_philo = ft_lstnew(i, data);
		if (!new_philo && write(2, "Error!\nNode allocation failed\n", 31))
			return (NULL);
		if (i == 1)
			(*table) = new_philo;
		new_philo->prev = prev;
		if (i > 1)
			prev->next = new_philo;
		prev = new_philo;
	}
	(*table)->prev = prev;
	prev->next = (*table);
	return (table);
}

static	t_data	*set_data(char **argv, int argc)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data && write(2, "Error!\nData allocation failed\n", 31))
		return (NULL);
	data->n_phil = ft_atos(argv[0]);
	data->die_time = ft_atos(argv[1]);
	if (data->n_phil == 1)
		return (only_one(data, data->die_time));
	data->eat_time = ft_atos(argv[2]);
	data->slp_time = ft_atos(argv[3]);
	data->meals = LLONG_MAX;
	if (argc == 6)
		data->meals = ft_atos(argv[4]);
	data->start_time = get_time(0);
	data->forks_sem = sem_open("/forks", O_CREAT, 0644, data->n_phil);
	data->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	data->game_over_sem = NULL;
	return (data);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	**table;

	if (write(1, "\e[38;5;208m", 12) && (argc < 5 || argc > 6))
		return (write(2, "Error!\nInvalid number of arguments\n", 36) - 35);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/game_over");
	sem_unlink("/meal_time");
	data = set_data(&argv[1], argc);
	if (!data)
		return (1);
	if (!(data->n_phil) || !(data->die_time) || !(data->eat_time)
		|| !(data->slp_time) || !(data->meals))
		return (write(2, "Error!\nInvalid argument\n", 25) + ft_free_all(data));
	table = set_table(data);
	if (!table)
		return (free(data), 1);
	data->table = table;
	if (philosophers(table, *data))
		return (ft_free_all(data));
	wait_processes(*data);
	return (ft_free_all(data) - 1);
}
