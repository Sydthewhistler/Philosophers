#include "../include/philo.h"

int	parse_args(int argc, char **argv, t_p *p)
{
	if ((argc == 5 || argc == 6) && numeric(argv, 0, 1))
	{
		p->all.total = ft_atoi(argv[1]);
		p->all.die = ft_atoi(argv[2]);
		p->all.eat = ft_atoi(argv[3]);
		p->all.sleep = ft_atoi(argv[4]);
		p->all.m_eat = -1;
		if (argc == 6)
			p->all.m_eat = ft_atoi(argv[5]);
		if (p->all.total <= 0 || p->all.die <= 0 || p->all.eat <= 0 \
			|| p->all.sleep <= 0)
			return (0);
		return (1);
	}
	return (0);
}

void	init_mutex(t_p *p)
{
	pthread_mutex_init(&p->all.write_mutex, NULL);
	pthread_mutex_init(&p->all.dead, NULL);
	pthread_mutex_init(&p->all.time_eat, NULL);
	pthread_mutex_init(&p->all.finish, NULL);
}

int	initialize(t_p *p)
{
	int	i;

	i = 0;
	p->all.start_t = actual_time();
	p->all.stop = 0;
	p->all.nb_p_finish = 0;
	init_mutex(p);
	while (i < p->all.total)
	{
		p->ph[i].id = i + 1;
		p->ph[i].ms_eat = p->all.start_t;
		p->ph[i].nb_eat = 0;
		p->ph[i].finish = 0;
		p->ph[i].r_f = NULL;
		pthread_mutex_init(&p->ph[i].l_f, NULL);
		if (p->all.total == 1)
			return (1);
		if (i == p->all.total - 1)
			p->ph[i].r_f = &p->ph[0].l_f;
		else
			p->ph[i].r_f = &p->ph[i + 1].l_f;
		i++;
	}
	return (1);
}
