/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:58:45 by scavalli          #+#    #+#             */
/*   Updated: 2026/05/07 12:44:25 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/header.h"

long	time_c(t_targs *thread_args)
{
	struct timeval	now;
	long			seconds;
	long			useconds;
	long			msec;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - thread_args->start_time.tv_sec;
	useconds = now.tv_usec - thread_args->start_time.tv_usec;
	msec = (seconds * 1000) + (useconds / 1000);
	return (msec);
}

void	display_status(long msec, t_targs *thread_args, char *str)
{
	pthread_mutex_lock(&thread_args->is_dead->mutex);
	if (thread_args->is_dead->end != true || ft_strcmp("\033[1;31mdied\033[0m",
			str) == 0)
	{
		printf("%ldms %d %s\n", msec, thread_args->id + 1, str);
		pthread_mutex_unlock(&thread_args->is_dead->mutex);
		return ;
	}
	pthread_mutex_unlock(&thread_args->is_dead->mutex);
	return ;
}

void	error(char *str)
{
	printf("%s\n", str);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	int		minus;
	int		nb;

	i = 0;
	minus = 1;
	nb = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus = -minus;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (nb * minus);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}