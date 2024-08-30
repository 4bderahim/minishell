#include "minishell.h"
// rl_on_new_line();
// rl_replace_line("", 0);// fix compiling 

void signal_handler(int signo) 
{
    if (signo == SIGINT)
        {


            // rl_on_new_line();
            
        //    rl_on_new_line();
           write(1, "\n", 1);
           rl_on_new_line();
           //rl_replace_line("", 0);
           rl_redisplay();


            }
    if (signo == SIGQUIT)
        {
            // rl_redisplay();
            // rl_on_new_line();
          //  rl_replace_line("", 0);// fix compiling 
            // rl_on_new_line();
            exit(1);
        }
    
}

void setup_signal_handlers() 
{
    // struct sigaction sa;
    // sa.sa_handler = signal_handler;
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART;
     signal(SIGINT, signal_handler);
     signal(SIGQUIT, signal_handler);


    // sigaction(SIGINT, &sa, NULL);

    // sigaction(SIGTSTP, &sa, NULL);
    // sigaction(SIGQUIT, &sa, NULL);
}
void reset_signal_handlers() 
{
     signal(SIGINT, SIG_DFL);
     signal(SIGQUIT, SIG_DFL);
     signal(SIGTSTP, SIG_DFL);
};