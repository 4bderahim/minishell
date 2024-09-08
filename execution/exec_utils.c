
#include "minishell.h"

void	handle_sigs(int sig)
{
	if (sig == SIGINT)
		printf("\n");
	if (sig == SIGQUIT)
		printf("QUIT 3:\n");
}

void	ignore_sigs(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	exiting_execution_loop(t_vars *vars, t_all *all)
{
	wait_ps(vars->pids, all);
	setup_signal_handlers();
	close(all->_vars->pr_fd);
}
