/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:41:56 by stescaro          #+#    #+#             */
/*   Updated: 2024/02/29 10:31:35 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdint.h>
# include <limits.h>
# include <semaphore.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>

typedef struct s_data
{
	sem_t				*forks_sem;
	sem_t				*print_sem;
	sem_t				*game_over_sem;
	size_t				start_time;
	size_t				n_phil;
	size_t				die_time;
	size_t				eat_time;
	size_t				slp_time;
	size_t				meals;
	struct s_philo		**table;
}						t_data;

typedef struct s_philo
{
	t_data			*data;
	sem_t			*meal_time_sem;
	pid_t			pid;
	pthread_t		tid;
	size_t			meal_time;
	size_t			id;
	size_t			meals_eaten;
	struct s_philo	*prev;
	struct s_philo	*next;
	int				stop;
}						t_philo;

t_data		*only_one(t_data *data, size_t die_time);
short		philosophers(t_philo **table, t_data d);
void		ft_print_status(t_philo *philo, char *str);
t_philo		*ft_lstnew(size_t i, t_data *d);
void		ft_lstclear(t_philo **table, t_data d);
size_t		ft_atos(char *str);
size_t		get_time(size_t start);
int			ft_free_all(t_data *data);

#endif
