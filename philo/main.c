/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:37:59 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/13 11:11:36 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
number_of_philosophers 
time_to_die 
time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]
*/

#include "philosophers.h"

static t_philo	**set_table(t_data *d)
{
	t_philo		**table;
	t_philo		*new_philo;
	t_philo		*prev;
	size_t		i;

	table = malloc(sizeof(t_philo *) * d->n_phil);
	if (!table && write(2, "Error!\nTable allocation failed\n", 32))
		return (NULL);
	prev = NULL;
	i = 0;
	while (++i <= d->n_phil)
	{
		new_philo = ft_lstnew(i, d);
		if (!new_philo && write(2, "Error!\nNode allocation failed\n", 31))
			return (ft_lstclear(table, d->n_phil), NULL);
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
	if ((!(data->n_phil) || !(data->die_time) || !(data->eat_time)
			|| !(data->slp_time) || !(data->meals))
		&& write(2, "Error!\nInvalid argument\n", 25))
		return (free(data), NULL);
	data->start_time = get_time(0);
	if ((pthread_mutex_init(&data->game_over_mutex, NULL)
			|| pthread_mutex_init(&data->satisfied_mutex, NULL)
			|| pthread_mutex_init(&data->print_mutex, NULL))
		&& write(2, "Error!\nMutex initialization failed\n", 36))
		return (free(data), NULL);
	return (data);
}

/*
static void	print_list(t_data *data)
{
	printf("number of philosophers: %lu\n", data->init);
	printf("time to die: %lu\n", data->die_time);
	printf("time to eat: %lu\n", data->eat_time);
	printf("time to sleep: %lu\n", data->slp_time);
	printf("times need to eat: %lu\n", data->times);
	for (int i = 0; i < 2 * data->tot; i++)
	{
		if (!(i % 2))
		{
			printf("philosopher number: %lu\n", data->phil->n);
			printf("times philosopher need to eat: %lu\n", data->phil->eat);
			printf("philosopher status: %u\n", data->phil->status);
		}
		else
			printf("Fork status: %u\n", data->phil->status);
		data->phil = data->phil->next;
	}
}
*/

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	**table;
	size_t	i;
	t_philo	*philo;

	if (write(1, "\e[38;5;208m", 12) && (argc < 5 || argc > 6))
		return (write(2, "Error!\nInvalid number of arguments\n", 36));
	data = set_data(&argv[1], argc);
	if (!data)
		return (1);
	data->game_over = 0;
	data->satisfied = 0;
	table = set_table(data);
	if (!table || philosophers(data, table))
		return (free(data), ft_free_all(data, table));
	i = 0;
	philo = *table;
	while (i < data->n_phil)
	{
		pthread_join(philo->thread_id, NULL);
		philo = philo->next;
		i++;
	}
	usleep(data->die_time * 1000 + 100);
	return (ft_free_all(data, table) - 1);
}
/*
int	main()
{
	t_data	*data;
	char **argv;
	int argc = 6;
	
	argv = malloc(sizeof(char *) * 6);
	argv[0] = "5";
	argv[1] = "11110";
	argv[2] = "1110";
	argv[3] = "1110";
	argv[4] = "3";
	argv[5] = NULL;
	if (argc < 5 || argc > 6)
		exit(write(2, "Error!\nInvalid number of arguments\n", 36));
	data = set_data(&argv[0], argc);
	print_list(data);
	return (0);
}
*/
