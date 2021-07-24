#include "stdiofs/util/connection_monitor.h"

#include <pthread.h>
#include <poll.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct cmon_context
{
    int readfd;
    int writefd;
    cmon_onerror_fn * onerror;
    void * user_data;
};

static void *
cmon_run(void * user_data)
{
    struct cmon_context * context = user_data;

    struct pollfd fds[2];
    fds[0].fd = context->readfd;
    fds[0].events = 0;
    fds[1].fd = context->writefd;
    fds[1].events = 0;

    bool finished = false;
    while (!finished)
    {
        int rc = poll(fds, 2, -1);
        finished = (0 < rc);
    }

    context->onerror(context->user_data);

    free(context);
    return NULL;
}

void
cmon_monitor(
    int readfd,
    int writefd,
    cmon_onerror_fn * onerror,
    void * user_data)
{
    struct cmon_context * context = malloc(sizeof(struct cmon_context));
    context->readfd = readfd;
    context->writefd = writefd;
    context->onerror = onerror;
    context->user_data = user_data;

    pthread_t thread;
    int rc = pthread_create(&thread, NULL, &cmon_run, context);
    if (0 != rc)
    {
        fprintf(stderr, "error: failed to install connection monitor\n");
        free(context);
        return;
    }

    rc = pthread_detach(thread);
    if (0 != rc)
    {
        fprintf(stderr, "error: failed to detach connection monitor\n");
        return;
    }
}
