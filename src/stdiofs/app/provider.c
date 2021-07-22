#define _GNU_SOURCE

#include "stdiofs/passthroughfs.h"
#include "stdiofs/fs/stub.h"
#include "stdiofs/rpc/connection.h"


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct context
{
    char * path;
    int command_index;
    int result;
    bool show_help;
};

static void
print_usage(void)
{
    printf(
        "stdiofs_provider, (C) 2021 Falk Werner <github.com/falk-werner>\n"
        "Provider for stdiofs\n"
        "\n"
        "Usage:\n"
        "\tstdiofs_provider [-p <path>] -- <command> [<args>...]\n"
        "\n"
        "Options:\n"
        "\t-p, --path - path to provide (default: current working directory)\n"
        "\t-h, --help - print this message\n"
        "\n"
        "Example:\n"
        "\tstdiofs_provider -p /tmp sshpass -p secret ssh user@host stdiofs -f -s /path/to/mountpoint\n"
    );
}

static void
parse_args(
    struct context * ctx,
    int argc,
    char * argv[])
{
    static struct option const options[] =
    {
        {"path", required_argument, NULL, 'p'},
        {"help", no_argument      , NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    ctx->result = EXIT_SUCCESS;
    ctx->show_help = false;
    ctx->command_index = 0;
    ctx->path = get_current_dir_name();

    opterr = 0;
    optind = 0;

    bool finished = false;
    while (!finished)
    {
        int option_index = 0;
        int c = getopt_long(argc, argv, "p:h", options, &option_index);
        switch (c)
        {
            case -1:
                finished = true;
                break;
            case 'p':
                free(ctx->path);
                ctx->path = realpath(optarg, NULL);
                if (NULL == ctx->path)
                {
                    fprintf(stderr, "error: cannot resolve path\n");
                    ctx->result = EXIT_FAILURE;
                    ctx->show_help = true;
                    finished = true;
                }
                break;
            case 'h':
                ctx->show_help = true;
                finished = true;
                break;
            default:
                fprintf(stderr, "error: unknown argument\n");
                ctx->result = EXIT_FAILURE;
                ctx->show_help = true;
                finished = true;
                break;
        }
    }

    ctx->command_index = optind;
    if ((!ctx->show_help) && (ctx->command_index >= argc))
    {
        fprintf(stderr, "error: missing command\n");
        ctx->result = EXIT_FAILURE;
        ctx->show_help = true;
    }    
}

static int
context_cleanup(
    struct context * ctx)
{
    int result = ctx->result;
    free(ctx->path);
    return result;
}

static void
run(
    struct context * ctx,
    int argc,
    char * argv[])
{
    int stdin_pipes[2];
    int rc = pipe(stdin_pipes);
    if (rc != 0)
    {
        fprintf(stderr, "error: failed to create pipe\n");
        ctx->result = EXIT_FAILURE;
        return;
    }

    int stdout_pipes[2];
    rc = pipe(stdout_pipes);
    if (rc != 0)
    {
        fprintf(stderr, "error: failed to create pipe\n");
        ctx->result = EXIT_FAILURE;
        return;
    }

    pid_t id = fork();
    if (0 < id)
    {
		close(stdin_pipes[0]);
		close(stdout_pipes[1]);

        struct rpc_connection connection;
        rpc_connection_init(&connection, stdout_pipes[0], stdin_pipes[1], 1);

        struct passthroughfs * fs = passthroughfs_create(ctx->path);
        struct fs_stub * stub = fs_stub_create(&connection, passthroughfs_get_operations(fs), fs);

        while (0 == fs_stub_service(stub))
        {
            // loop
        }

        fs_stub_release(stub);
        passthroughfs_release(fs);
        rpc_connection_cleanup(&connection);

		int status;
		waitpid(id, &status, 0);
    }
    else if (0 == id)
    {
		close(STDIN_FILENO);
		dup2(stdin_pipes[0], STDIN_FILENO);
		close(stdin_pipes[1]);

		close(STDOUT_FILENO);
		dup2(stdout_pipes[1], STDOUT_FILENO);
		close(stdout_pipes[0]);

        char const * command = argv[ctx->command_index]; 
		rc = execvp(command, &argv[ctx->command_index]);
		fprintf(stderr, "error: failed to execute command: %s\n", strerror(errno));
        return;
    }
    else
    {
        fprintf(stderr, "error: failed to create pipe\n");
        ctx->result = EXIT_FAILURE;
        return;
    }

}

int main(int argc, char * argv[])
{
    struct context ctx;
    parse_args(&ctx, argc, argv);

    if (!ctx.show_help)
    {
        run(&ctx, argc, argv);
    }
    else
    {
        print_usage();
    }

    return ctx.result;
}