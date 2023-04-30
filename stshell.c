
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <strings.h>
#include <sys/stat.h>
# include <bits/sigaction.h>

#define COMMAND_LEN 1024
#define ARG_LEN 64
#define	STDIN	0
#define	STDOUT	1

volatile sig_atomic_t isRunning = 1;
struct sigaction sa;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        //closing proccess
        printf("\n");
        isRunning = 0;
    }
}

void runCommand(char** args, int fd_in, int fd_out) {
    pid_t pid = fork();

    if (pid == 0) {//child process
        if (fd_in != STDIN) {
            dup2(fd_in, STDIN);
            close(fd_in);
        }

        if (fd_out != STDOUT) {
            dup2(fd_out, STDOUT);
            close(fd_out);
        }
        execvp(args[0], args);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        perror("Fork Error");
    }
    else {
        int status;
        do {
            waitpid(pid, &status, 0);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

/*
O_APPEND
              The file is opened in append mode.  Before  each  write(2),  the
              file  offset  is  positioned  at the end of the file, as if with
              lseek(2).  The modification of the file offset and the write op‐
              eration are performed as a single atomic step.

              O_APPEND  may lead to corrupted files on NFS filesystems if more
              than one process appends data to a file at once.   This  is  be‐
              cause  NFS  does  not support appending to a file, so the client
              kernel has to simulate it, which can't be done  without  a  race
              condition.
O_CREAT
              If pathname does not exist, create it as a regular file.

              The owner (user ID) of the new file is set to the effective user
              ID of the process.

              The  group ownership (group ID) of the new file is set either to
              the effective group ID of the process (System V semantics) or to
              the group ID of the parent directory (BSD semantics).  On Linux,
              the behavior depends on whether the set-group-ID mode bit is set
              on  the parent directory: if that bit is set, then BSD semantics
              apply; otherwise, System V semantics apply.  For  some  filesys‐
              tems,  the behavior also depends on the bsdgroups and sysvgroups
              mount options described in mount(8)).

              The mode argument specifies the file mode bits be applied when a
              new  file  is  created.   This  argument  must  be supplied when
              O_CREAT or O_TMPFILE is specified in flags; if  neither  O_CREAT
              nor O_TMPFILE is specified, then mode is ignored.  The effective
              mode is modified by the process's umask in the usual way: in the
              absence  of  a  default  ACL,  the  mode  of the created file is
              (mode & ~umask).  Note that this mode applies only to future ac‐
              cesses of the newly created file; the open() call that creates a
              read-only file may well return a read/write file descriptor.

S_IRWXU  00700 user (file owner) has read,  write,  and  execute permission

O_TRUNC
              If  the file already exists and is a regular file and the access
              mode allows writing (i.e., is O_RDWR or  O_WRONLY)  it  will  be
              truncated to length 0.  If the file is a FIFO or terminal device
              file, the O_TRUNC flag is ignored.   Otherwise,  the  effect  of
              O_TRUNC is unspecified.
*/
void parse_command(char* command, char** args, int* num_args, int* input_fd, int* output_fd) {
    char* token = strtok(command, " ");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            *input_fd = open(token, O_RDONLY);
            if (*input_fd < 0) {
                perror("open");
                return;
            }
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            *output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (*output_fd < 0) {
                perror("open");
                return;
            }
        }
        else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " ");
            *output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
            if (*output_fd < 0) {
                perror("open");
                return;
            }
        }
        else if (strcmp(token, "|") == 0) {
            int pipe_fds[2];
            pipe(pipe_fds);
            runCommand(args, *input_fd, pipe_fds[1]);
            close(pipe_fds[1]);
            *num_args = 0;
            *input_fd = pipe_fds[0];
        }
        else {
            args[*num_args] = token;
            (*num_args)++;
        }

        token = strtok(NULL, " ");
    }
    args[*num_args] = NULL;
}

int main() {
    bzero(&sa, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    char command[COMMAND_LEN];
    char* args[ARG_LEN];
    int num_args = 0;
    int fd_in, fd_out;

    while (isRunning) {
        num_args = 0;
        fd_in = STDIN;
        fd_out = STDOUT;
        printf("stshell> ");
        fgets(command, COMMAND_LEN, stdin);
        command[strcspn(command, "\n")] = '\0';
        parse_command(command, args, &num_args, &fd_in, &fd_out);
        if(!isRunning)num_args = 0;
        if (num_args > 0) {
            runCommand(args, fd_in, fd_out);
        }

        if (fd_in != STDIN) {
            close(fd_in);
        }

        if (fd_out != STDOUT) {
            close(fd_out);
        }
    }
    return 0;
}
