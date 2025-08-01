#ifndef LUMBER_H
#define LUMBER_H

// "-DLOGGING" to init Lumber or omit to not log
#ifndef LOGGING
#define LOGGING 0
#endif

// " -DPROGNAME=\"...\" " to declare the Programe Name
#ifndef PROGNAME
#define PROGNAME "main"
#endif

// " -DLOG_DIR=\"...\" " to declare the Log Directory
#ifndef LOG_DIR
#define LOG_DIR "logs"
#endif

// " -DFORMAT=\"...\" " to declare the format option for logging
#ifndef FORMAT
#define FORMAT 0
#endif

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)

static const char *win_basename(const char *path) {
  const char *p = strrchr(path, '\\');
  if (!p) {
    p = strrchr(path, '/');
  }

  return p ? p + 1 : path;
}
#define basename(x) win_basename(x)
#else
#include <libgen.h>
#include <sys/stat.h>
#endif

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static FILE *log_file = NULL;

static void init_log(const char *exe_path) {
  if (log_file) {
    return;
  }

  if (mkdir(LOG_DIR, 0755) < 0 && errno != EEXIST) {
    fprintf(stderr,
            "[lumber.h] ERROR: Unable to create/access Log Directory,\n"
            "[lumber.h]     Attempted to create at: %s/",
            LOG_DIR);
    return;
  }

  const char *exe = basename((char *)exe_path);

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char fname[PATH_MAX];

  snprintf(fname, sizeof(fname),
           LOG_DIR "/%04d%02d%02d_%02d%02d%02d_%s_log.txt", tm.tm_year + 1900,
           tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, exe);

  log_file = fopen(fname, "a");

  if (!log_file) {
    fprintf(stderr, "[lumber.h] ERROR: Could not open '%s': %s\n", fname,
            strerror(errno));
  }
}

static void log_msg(const char *exe_name, const char *msg) {
  if (!LOGGING) {
    return;
  }

  if (!log_file) {
    init_log(exe_name);
  }

  if (log_file) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    switch (FORMAT) {
    case 0:
      fprintf(log_file, "[%04d-%02d-%02d %02d:%02d:%02d] %s", tm.tm_year + 1900,
              tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, msg);
      break;
    case 1:
      fprintf(log_file, "%s", msg);
      break;
    default:
      fprintf(stderr,
              "[lumber.h] ERROR: Unknown Format %d\n"
              "[lumber.h]     Attempted to Log: %s",
              FORMAT, msg);
      fflush(stderr);
      return;
      break;
    }

    fflush(log_file);
  }
}

#define log(s) log_msg(PROGNAME, (s))

#define logf(fmt, ...)                                                         \
  do {                                                                         \
    char _logbuf[PATH_MAX];                                                    \
    int _ = snprintf(_logbuf, sizeof(_logbuf), fmt, __VA_ARGS__);              \
    (void)_;                                                                   \
    log(_logbuf);                                                              \
  } while (0)

#endif
