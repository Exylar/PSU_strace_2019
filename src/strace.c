/*
** EPITECH PROJECT, 2019
** PSU_strace_2019
** File description:
** strace
*/

#include "strace.h"
#include "syscall.h"

static void display_arguments(size_t i, struct user_regs_struct regs)
{
    printf("%s(", sys[i].str);
    for (size_t count = 0; count < sys[i].nb_arg; count++) {
        if (count == 0)
            printf("0x%llx", regs.rdi);
        if (count == 1)
            printf("0x%llx", regs.rsi);
        if (count == 2)
            printf("0x%llx", regs.rdx);
        if (count == 3)
            printf("0x%llx", regs.r10);
        if (count == 4)
            printf("0x%llx", regs.r8);
        if (count == 5)
            printf("0x%llx", regs.r9);
        if (count + 1 != sys[i].nb_arg)
            printf(", ");
    }
    printf(") = 0x%llx\n", regs.rax);
}

static void display_arguments_detail(size_t i, struct user_regs_struct regs,
pid_t pid)
{
    printf("%s(", sys[i].str);
    for (size_t count = 0; count < sys[i].nb_arg; count++) {
        if (count == 0)
            print_type(i, count, regs.rdi, pid);
        if (count == 1)
            print_type(i, count, regs.rsi, pid);
        if (count == 2)
            print_type(i, count, regs.rdx, pid);
        if (count == 3)
            print_type(i, count, regs.r10, pid);
        if (count == 4)
            print_type(i, count, regs.r8, pid);
        if (count == 5)
            print_type(i, count, regs.r9, pid);
        if (count + 1 != sys[i].nb_arg)
            printf(", ");
    }
    printf(") = ");
    print_type(i, 6, regs.rax, pid);
    printf("\n");
}

static void display_information(pid_t pid, args_t *args)
{
    struct user_regs_struct regs;

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    for (size_t i = 0; i < 313; i++) {
        if (i == regs.orig_rax && args->s == 0)
            display_arguments(i, regs);
        else if (i == regs.orig_rax && args->s == 1)
            display_arguments_detail(i, regs, pid);
    }
    ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
}

int strace(args_t *args)
{
    pid_t pid;
    int status;
    char *arg[] = {args->path, NULL};

    pid = fork();
    if (pid == -1)
        return (EXIT_FAILURE);
    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        if (execvp(args->path, arg) == -1)
            perror("execvp: ");
    }
    else {
        while (waitpid(pid, &status, 0) && !WIFEXITED(status))
            display_information(pid, args);
    }
    printf("exit_group(%d) = ?\n", WEXITSTATUS(status));
    printf("+++ exited with %d +++\n", WEXITSTATUS(status));
    return (0);
}

int strace_by_pid(args_t *args)
{
    int status;
    pid_t pid = (pid_t)atoi(args->p);

    if (ptrace(PTRACE_ATTACH, pid, 0, 0) != 0)
        return (84);
    while (waitpid(pid, &status, 0) && !WIFEXITED(status))
        display_information(pid, args);
    printf("exit_group(%d) = ?\n", WEXITSTATUS(status));
    printf("+++ exited with %d +++\n", WEXITSTATUS(status));
    return (0);
}