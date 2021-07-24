#ifndef UTIL_CONNECTION_MONITOR
#define UTIL_CONNECTION_MONITOR

#ifdef __cplusplus
extern "C"
{
#endif

typedef void
cmon_onerror_fn(
    void * user_data);

extern void 
cmon_monitor(
    int readfd,
    int writefd,
    cmon_onerror_fn * onerror,
    void * user_data);

#ifdef __cplusplus
}
#endif

#endif