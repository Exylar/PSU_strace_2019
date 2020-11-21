/*
** EPITECH PROJECT, 2020
** PSU_strace_2019
** File description:
** print
*/

#include "strace.h"
#include "syscall.h"

void *get_num_args(size_t i, size_t count)
{
    void *res = NULL;

    arg_t arg[] = {
        {1, (void *)sys[i].arg1},
        {2, (void *)sys[i].arg2},
        {3, (void *)sys[i].arg3},
        {4, (void *)sys[i].arg4},
        {5, (void *)sys[i].arg5},
        {6, (void *)sys[i].arg6},
        {7, (void *)sys[i].t_return}
    };
    for (size_t j = 0; j < 7; j++) {
        if (arg[j].number == count + 1)
            res = arg[j].arg;
    }
    return res;
}

static void print_char_ptr(unsigned long long int param, pid_t pid)
{
    long pt = -1;

    printf("\"");
    for (int i = 0; (char)pt != '\0' && i < 42; i++) {
        if ((pt = ptrace(PTRACE_PEEKTEXT, pid, param, NULL)) == -1) {
            printf("ptrace PTRACE_PEEKDATA error: %s\"", strerror(errno));
            return;
        }
        if ((((char)pt >= ' ' && (char)pt <= '~') || (char)pt == '\n') &&
            (char)pt != '\0')
            ((char)pt == '\n') ? printf("\\n") : printf("%c", (char)pt);
        else if ((char)pt != '\n' && (char)pt != '\0')
            printf("\\%o", (char)pt);
        param++;
    }
    printf("\"");
}

void print_struct(void *param)
{
    struct stat	*st = (struct stat *)param;

    if (st == NULL || param == NULL) {
        printf("{}");
        return;
    }
    printf("{st_mode=%d, st_size=%lx, ...}", st->st_mode, st->st_size);
}

void print_type(size_t i, size_t count, unsigned long long int param, pid_t pid)
{
    void *arg = get_num_args(i, count);

    if (arg == (void *)INT || arg == (void *)SSIZE_T)
        printf("%d", (int)(param));
    if (arg == (void *)LONG)
        printf("%ld", (long)param);
    if (arg == (void *)ULONG || arg == (void *)SIZE_T)
        printf("%lld", param);
    if (arg == (void *)STRING)
        print_char_ptr(param, pid);
    if (arg == (void *)STRUCT)
        print_struct(&param);
    if (arg == (void *)DEFAULT || arg == (void *)POINTER)
        printf("0x%llx", param);
    if (arg == (void *)NONE && count == 6)
        printf("?");
}
