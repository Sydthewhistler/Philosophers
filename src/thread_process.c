/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:34:19 by scavalli          #+#    #+#             */
/*   Updated: 2026/05/07 12:44:20 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/header.h"

int	is_dead(struct timeval meal_date, t_targs *thread_args)
{
	long	current_time_ms;
	long	last_meal_ms;

	current_time_ms = (meal_date.tv_sec * 1000) + (meal_date.tv_usec / 1000);
	last_meal_ms = (thread_args->last_meal.tv_sec * 1000)
		+ (thread_args->last_meal.tv_usec / 1000);
	if (current_time_ms - last_meal_ms >= thread_args->time_to_die)
	{
		pthread_mutex_lock(&thread_args->is_dead->mutex);
		thread_args->is_dead->end = true;
		pthread_mutex_unlock(&thread_args->is_dead->mutex);
		display_status(time_c(thread_args), thread_args,
			"\033[1;31mdied\033[0m");
		pthread_mutex_unlock(&thread_args->neighbor->data->mutex);
		pthread_mutex_unlock(&thread_args->data->mutex);
		return (1);
	}
	return (0);
}

int	check_ifend(t_targs *thread_args, int is_second_mutex)
{
	//printf("ICI\n");
	pthread_mutex_lock(&thread_args->is_dead->mutex);
	//printf("LOCKED\n");
	if (thread_args->is_dead->end == true)
	{
		if(is_second_mutex != -1)
			pthread_mutex_unlock(&thread_args->data->mutex);
		if (is_second_mutex == 1)
			pthread_mutex_unlock(&thread_args->neighbor->data->mutex);
		pthread_mutex_unlock(&thread_args->is_dead->mutex);
		////printf("UNLOCKED\n");
		return (1);
	}
	pthread_mutex_unlock(&thread_args->is_dead->mutex);
	////printf("UNLOCKED\n");
	return (0);
}

void	*thread_process(void *args)
{
	t_targs			*thread_args;
	struct timeval	meal_date;

	thread_args = (t_targs *)args;
	thread_args->data->fork_status = 1;
	while (1)
	{
		display_status(time_c(thread_args), thread_args, "is thinking");
		if (check_ifend(thread_args, -1) == 1) //verif if stop
			return (NULL);
		pthread_mutex_lock(&thread_args->data->mutex); //prend sa fourchette
		if (check_ifend(thread_args, 0) == 1) //verif if stop
			return (NULL);
		display_status(time_c(thread_args), thread_args, "has taken a fork");
		// if (check_ifend(thread_args, 0) == 1) //verif if stop
		// 	return (NULL);
		pthread_mutex_lock(&thread_args->neighbor->data->mutex); //prend fourchette voisin. PRBLM ICI
		if (check_ifend(thread_args, 1) == 1) // verif if stop
			return (NULL);
		display_status(time_c(thread_args), thread_args, "has taken a fork");
		gettimeofday(&meal_date, NULL);
		if (is_dead(meal_date, thread_args) == 1) //verifie si philo vivant
			return (NULL);
		display_status(time_c(thread_args), thread_args, "is eating");
		thread_args->last_meal = meal_date; //retiens le dernier repas
		usleep((thread_args->time_to_eat) * 1000); //mange
		pthread_mutex_unlock(&thread_args->neighbor->data->mutex); //libere fourchette voisin
		pthread_mutex_unlock(&thread_args->data->mutex); //libere sa fouchette
		display_status(time_c(thread_args), thread_args, "is sleeping");
		usleep((thread_args->time_to_sleep) * 1000); //dort
	}
	return (NULL);
}
