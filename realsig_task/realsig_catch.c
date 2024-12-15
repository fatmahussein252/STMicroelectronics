#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/* Handler for signals established using SA_SIGINFO */
static void siginfoHandler (int sig, siginfo_t * si, void *ucontext)
{
 
  printf ("caught signal %d\n", sig);
  printf ("si_signo=%d, si_code=%d (%s), ", si->si_signo, si->si_code, (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "Not real signal");
  printf ("si_value=%d\n", si->si_value.sival_int);
  printf ("si_pid=%ld, si_uid=%ld\n", (long) si->si_pid, (long) si->si_uid);
  if(si->si_value.sival_int == 0){
  	printf("Exiting without core dump...\n");
        exit(EXIT_SUCCESS);  // Clean termination without core dump 
        }
  else{ 
  	printf("Generating core dump...\n");
        abort();  // Generates core dump and terminates
        }
  
}

int
main (int argc, char *argv[])
{
  struct sigaction sa;

  printf ("%s: PID is %ld\n", argv[0], (long) getpid ());
  
/* Establish handler for the real signal and SIGINT signal. */
  sa.sa_sigaction = siginfoHandler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset (&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGINT);
  

  sigaction (SIGINT, &sa, NULL);
/* Wait for incoming signals */
  while (1){
    printf("waiting for incoming signal\n");
    pause ();
    }

}
