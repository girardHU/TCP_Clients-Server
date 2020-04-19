#include "connect.h"

void err_n_die(int* sock, const char* fmt, ...) {
  int errno_save;
  va_list ap;

  // all system calls can set errno, so we save it now
  errno_save = errno;

  // print out the fmt+args to standard out
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stdout, "\n");
  fflush(stdout);

  // print out error message if errno was set
  if (errno_save != 0) {
    fprintf(stdout, "errno = %d : %s\n", errno_save, strerror(errno_save));
    fprintf(stdout, "\n");
    fflush(stdout);
  }
  va_end(ap);
  //close the socket
  if (sock != NULL)
    close(*sock);

  // terminate with an error
  exit(1);
}