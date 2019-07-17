#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/capability.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
	"/bin/bash",
	NULL
};

int child_main(void* args){
	printf("in child process\n");
	printf("eUID = %ld; eGID = %ld; ", (long) geteuid(), (long) getegid());
	cap_t caps;
	caps = cap_get_proc();
	printf("capability: %s\n", cap_to_text(caps, NULL));
	sethostname("NewNameSpace", 12);
	execv(child_args[0],child_args);
	return 1;
}

int main(){
	printf("start main function\n");
	int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUSER | CLONE_NEWNET | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
	waitpid(child_pid, NULL, 0);
	printf("exit\n");
	return 0;
}
