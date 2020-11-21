/*
** EPITECH PROJECT, 2020
** PSU_strace_2019
** File description:
** main
*/

#include "strace.h"

static void args_def(args_t *args)
{
    args->h = 0;
    args->p = NULL;
    args->s = 0;
    args->path = NULL;
}

static int check_args(char **argv, int i, args_t *args, int argc)
{
    if (strcmp(argv[i], "-s") == 0) {
        args->s = 1;
        return (1);
    }
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        args->h = 1;
        return (1);
    }
    if (strcmp(argv[i], "-p") == 0) {
        if (i + 1 >= argc)
            return (84);
        args->p = malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
        args->p = strcpy(args->p, argv[i + 1]);
        i++;
        return (2);
    }
    return (0);
}

static int detect_args(int argc, char **argv, args_t *args)
{
    int tmp = 0;

    for (int i = 1; i < argc; i++) {
        tmp = check_args(argv, i, args, argc);
        if (tmp == 2)
            i++;
        if (tmp == 1 || tmp == 2)
            continue;
        else if (tmp == 84)
            return (84);
        if (args->path != NULL)
            return (84);
        args->path = malloc(sizeof(char) * (strlen(argv[i]) + 1));
        args->path = strcpy(args->path, argv[i]);
    }
    return (0);
}

int parse_arg(int argc, char **argv)
{
    args_t *args = malloc(sizeof(args_t));
    int res;

    if (argc < 2 || argc > 4) {
        free(args);
        return (84);
    }
    args_def(args);
    if (detect_args(argc, argv, args) == 84)
        return (84);
    if (args->h == 1) {
        printf("USAGE: ./strace [-s] [-p <pid>|<command>]\n");
        return (0);
    }
    if (args->p == 0)
        res = (strace(args));
    else
        res = (strace_by_pid(args));
    free(args->path);
    free(args);
    return res;
}
