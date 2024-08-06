// void execute_command_line(t_command *cmd_list) {
//     int pipe_count = count_pipes(cmd_list);
//     int total_processes = pipe_count + 1;
//     int pipes[total_processes - 1][2];
//     pid_t pids[total_processes];

//     for (int i = 0; i < total_processes - 1; i++) {
//         if (pipe(pipes[i]) == -1) {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }
//     }

//     for (int i = 0; i < total_processes; i++) {
//         pids[i] = fork();
//         if (pids[i] == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         } else if (pids[i] == 0) {
//             // Child process
//             setup_child_io(i, total_processes, pipes);
//             execute_single_command(cmd_list);
//             exit(EXIT_SUCCESS);
//         }
//         // Move to next command in the list
//         cmd_list = cmd_list->next;
//     }

//     // Parent process
//     close_all_pipes(pipes, total_processes - 1);
//     wait_for_children(pids, total_processes);
// }

// void setup_child_io(int index, int total_processes, int pipes[][2]) {
//     if (index > 0) {
//         // Not the first process, read from previous pipe
//         dup2(pipes[index - 1][0], STDIN_FILENO);
//     }
//     if (index < total_processes - 1) {
//         // Not the last process, write to next pipe
//         dup2(pipes[index][1], STDOUT_FILENO);
//     }

//     // Close all pipe fds in child
//     for (int i = 0; i < total_processes - 1; i++) {
//         close(pipes[i][0]);
//         close(pipes[i][1]);
//     }
// }

// void execute_single_command(t_command *cmd) {
//     handle_redirections(cmd);
//     handle_heredoc(cmd);
    
//     if (is_builtin(cmd->argv[0])) {
//         execute_builtin(cmd);
//     } else {
//         execvp(cmd->argv[0], cmd->argv);
//         perror("execvp");
//         exit(EXIT_FAILURE);
//     }
// }

// void handle_heredoc(t_command *cmd) {
//     if (cmd->heredoc_content) {
//         int pipefd[2];
//         if (pipe(pipefd) == -1) {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }
//         pid_t pid = fork();
//         if (pid == 0) {
//             // Child writes heredoc content
//             close(pipefd[0]);
//             write(pipefd[1], cmd->heredoc_content, strlen(cmd->heredoc_content));
//             close(pipefd[1]);
//             exit(EXIT_SUCCESS);
//         } else {
//             // Parent reads from pipe
//             close(pipefd[1]);
//             dup2(pipefd[0], STDIN_FILENO);
//             close(pipefd[0]);
//             waitpid(pid, NULL, 0);
//         }
//     }
// }

// void handle_redirections(t_command *cmd) {
//     // Handle input redirection
//     if (cmd->input_file) {
//         int fd = open(cmd->input_file, O_RDONLY);
//         if (fd == -1) {
//             perror("open");
//             exit(EXIT_FAILURE);
//         }
//         dup2(fd, STDIN_FILENO);
//         close(fd);
//     }

//     // Handle output redirection
//     if (cmd->output_file) {
//         int flags = O_WRONLY | O_CREAT | (cmd->append_output ? O_APPEND : O_TRUNC);
//         int fd = open(cmd->output_file, flags, 0644);
//         if (fd == -1) {
//             perror("open");
//             exit(EXIT_FAILURE);
//         }
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }
// }