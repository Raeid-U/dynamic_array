#ifndef LUMBER_H
#define LUMBER_H

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Change this if you ever want to write to another directory
#ifndef LOG_DIR
#define LOG_DIR "logs"
#endif

// Define executable name
#define __progname "main"

static FILE *log_file = NULL;

// Initialize the log (called once on first log message)
static void init_log(const char *exe_path) {
  if (log_file)
    return;

  // 1) Make sure logs/ exists (ignore if it already does)
  if (mkdir(LOG_DIR, 0755) < 0 && errno != EEXIST) {
    perror("mkdir log dir");
    return;
  }

  // 2) Only use the basename of the executable
  const char *exe = basename((char *)exe_path);

  // 3) Build a full path under logs/
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char fname[PATH_MAX];
  snprintf(fname, sizeof(fname),
           LOG_DIR "/%04d%02d%02d_%02d%02d%02d_%s_log.txt", tm.tm_year + 1900,
           tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, exe);

  // 4) Try opening it for append
  log_file = fopen(fname, "a");
  if (!log_file) {
    fprintf(stderr, "Could not open '%s': %s\n", fname, strerror(errno));
  }
}

// Append a timestamped message
static void log_msg(const char *exe_name, const char *msg) {
  if (!log_file)
    init_log(exe_name);
  if (log_file) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // OLD LOG FORMAT [timestamp] msg \n
    // fprintf(log_file, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n", tm.tm_year +
    // 1900,
    //      tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, msg);

    fprintf(log_file, "%s", msg);
    fflush(log_file);
  }
}

// Convenience macro: pass argv[0] or your own executable name
#define log(s) log_msg(__progname, (s))

// Convenience macro: standin for log with format specifiers
#define logf(fmt, ...)                                                         \
  do {                                                                         \
    char _logbuf[512];                                                         \
    int _ = snprintf(_logbuf, sizeof _logbuf, fmt, __VA_ARGS__);               \
    (void)_; /* silence unused‐result warning */                               \
    log(_logbuf);                                                              \
  } while (0)

// You’ll need to extern or define __progname yourself:
// e.g. in your main.c:
//   const char *__progname;
//   int main(int argc, char **argv) {
//     __progname = argv[0];
//     …
//   }
#endif
