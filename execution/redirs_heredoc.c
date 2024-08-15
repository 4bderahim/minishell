#include "minishell.h"

void redirections_set(t_all *all) 
{
    int fd;
    if (all->cmd->in_file)
    {
        fd = open(all->cmd->in_file, O_RDONLY);
        if (fd == -1) 
            ft_error(all);
        if (dup2(fd, STDIN_FILENO) < 0)
            ft_error(all);
        close(fd);
    }
    if (all->cmd->out_file || all->cmd->append_file) 
    {
        if (all->cmd->append_file)
            fd = open(all->cmd->out_file, O_APPEND, 0644);
        else
            fd = open(all->cmd->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            ft_error(all);
        if (dup2(fd, STDOUT_FILENO))
            ft_error(all);
        close(fd);
    }
}

void heredoc_pipe(t_all *all)
{
    int fd;
    int p[2];
    if (all->cmd->heredoc_delimiter == NULL)
        return ;
    if (pipe(p)<0)
        ft_error(all);
    fd = fork();
    if (fd == -1)
        ft_error(all);
    if (fd == 0)
    {
        close(p[0]);
        redirections_set(all);
        ft_write(all->cmd->heredoc_content, p[1]);
        close(p[1]);
        exit(1);
    }
    close(p[1]);
    if (dup2(p[0], STDIN_FILENO) < 0)
        ft_error(all);
    close(p[0]);
}

void redirect_in_out_to_pipe(int n_pipes, int index, int pipe[],int *pr_fd, t_all *all)
{
    if (index != 0)
    {
        if (dup2(*pr_fd, STDIN_FILENO) < 0)
            ft_error(all);
        close(*pr_fd);
    }
    if (index < n_pipes -1)
    {
        if (dup2(pipe[1],STDOUT_FILENO) < 0)
            ft_error(all);
        close(pipe[1]);
    }
}