#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[]) {
    int fd1[2]; //father->child
    int fd2[2]; //child->father
    int err_status=0;
    char buf;

    pipe(fd1);
    pipe(fd2);

    int pid = fork();
    if(pid < 0) { //error
        fprintf(2, "fork() error");
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        exit(1);
    } else if(pid == 0) { //child
        close(fd1[1]);
        close(fd2[0])

        if(read(fd1[0], &buf, 1) != 1) {
            fprintf(2, "child read() error!\n");
            err_status = 1;
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }

        if(write(fd2[1], &buf, 1) != 1) {
            fprintf(2, "child write() error!\n");
            err_status = 1;
        }

        close(fd1[0]);
        close(fd2[1]);
        exit(err_status);
    } else { //father
        close(fd1[0]);
        close(fd2[1]);

        if(write(fd1[1], &buf, 1) != 1) {
            fprintf(2, "parent write() error!\n");
            err_status = 1;
        }

        if(read(fd2[0], &buf, 1) != 1) {
            fprintf(2, "parent read() error!\n");
            err_status = 1;
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }

        close(fd1[1]);
        close(fd2[0]);
        exit(err_status);
    }
}