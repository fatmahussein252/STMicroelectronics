#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main (int argc, char *argv[])
{
  int sigData;
  long c_pid;
  union sigval sv;
// check number of arguments needed
  if (argc < 3)
    {
      printf
	("Usage Error:\nuse: ./rss <recieving_pid> <0 or 1 for core dump>\n");
      exit (1);
    }
/* Display sender PID and UID, so that they can be compared with the
corresponding fields of the siginfo_t argument supplied to the
handler in the receiving process */
  printf ("%s: PID is %ld, UID is %ld\n", argv[0], (long) getpid (),
	  (long) getuid ());

// convert pid to long data type
  char *endptr;
  c_pid = strtol (argv[1], &endptr, 10);

  if (*endptr != '\0')
    {
      printf ("Error: Invalid pid, non-numeric characters detected\n");
      exit (1);
    }

// convert sigData to intger
  sigData = atoi (argv[2]);
  if (sigData == 0)
    printf ("process %ld will terminate without core dump\n", c_pid);
  else
    printf ("process %ld will terminate with core dump\n", c_pid);

// send real signal for termination
  sv.sival_int = sigData;
  if (sigqueue (c_pid, SIGINT, sv) == -1)
    perror ("sigqueue");


  exit (EXIT_SUCCESS);
}
