#include "minishell.h"

void	heredoc_sig_(int signo)
{
	if (signo == SIGINT)
		g_signaled = 1;
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signaled = 1;
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_sigs(void)
{
	signal(SIGINT, handle_sigs);
	signal(SIGQUIT, handle_sigs);
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
}
