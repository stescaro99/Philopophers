/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 08:44:24 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/04 10:07:17 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdint.h>
# include <limits.h>

typedef struct s_data
{
	pthread_mutex_t	game_over_mutex;
	pthread_mutex_t	satisfied_mutex;
	pthread_mutex_t	print_mutex;
	size_t			n_phil;
	size_t			die_time;
	size_t			eat_time;
	size_t			slp_time;
	size_t			meals;
	size_t			satisfied;
	size_t			start_time;
	short			game_over;
}		t_data;

typedef struct s_philo
{
	pthread_mutex_t	fork_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_t		thread_id;
	pthread_t		thread2_id;
	t_data			*data;
	size_t			meal_time;
	size_t			id;
	size_t			eaten;
	struct s_philo	*prev;
	struct s_philo	*next;
	short			stop;
}		t_philo;

size_t	ft_atos(char *str);
size_t	get_time(size_t start);
void	ft_lstclear(t_philo **table, size_t num_philo);
t_philo	*ft_lstnew(size_t n, t_data *data);
short	ft_free_all(t_data *data, t_philo **table);
short	philosophers(t_data *data, t_philo **table);
t_data	*only_one(t_data *data, size_t die_time);
short	game_over(t_data *d);
void	set_game_over(t_data *d);
void	ft_print_status(t_data *d, size_t id, char *str);

#endif