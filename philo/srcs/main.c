#include "../include/philo.h"

int	ft_exit(char *str)
{
	ft_putstr_error("Error : ");
	ft_putstr_error(str);
	return (0);
}

int	check_death2(t_p *p)
{
	pthread_mutex_lock(&p->all.dead);
	if (p->all.stop)
	{
		pthread_mutex_unlock(&p->all.dead);
		return (1);
	}
	pthread_mutex_unlock(&p->all.dead);
	return (0);
}

void	stop(t_p *p)
{
	int	i;

	i = -1;
	while (!check_death2(p))
		ft_usleep(1);
	while (++i < p->all.total)
		pthread_join(p->ph[i].thread_id, NULL);
	pthread_mutex_destroy(&p->all.write_mutex);
	i = -1;
	while (++i < p->all.total)
		pthread_mutex_destroy(&p->ph[i].l_f);
	if (p->all.stop == 2)
		printf("Each philosopher ate %d time(s)\n", p->all.m_eat);
	free(p->ph);
}

int	main(int argc, char **argv)
{
	t_p		p;

	if (!(parse_args(argc, argv, &p)))
		return (ft_exit("Invalid Arguments\n"));
	p.ph = malloc(sizeof(t_philo) * p.all.total);
	if (!p.ph)
		return (ft_exit("Malloc returned NULL\n"));
	if (!initialize(&p) || !threading(&p))
	{
		free(p.ph);
		return (0);
	}
	stop(&p);
}
