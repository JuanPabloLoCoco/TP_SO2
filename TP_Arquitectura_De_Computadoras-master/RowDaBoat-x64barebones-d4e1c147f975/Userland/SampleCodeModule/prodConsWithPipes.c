#include <syscalls.h>
#include <stdlib.h>
#include <prodConsWithPipes.h>
#include <math.h>

void olvidameVersionSO()
{
    void** parg = (void**)malloc(sizeof(void*) * 2);
    exec(&student, parg, "student");
    exec(&so, parg, "so");
}


void student()
{
    char* msgs[4] = {
            "Quien es?",
            "Que vienes a buscar?",
            "Ya es tarde",
            "Porque yo ya entregue el tp"
    };
    char buffer[256] = {0};
    int so = openPipe("StudentToSO");
    int me = openPipe("SOToStudent");
    // syscall_read(me, buffer, 99);
    // printf("SO: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(so, msgs[0], strlen(msgs[0]) + 1);
    // syscall_read(me, buffer, 99);
    // printf("SO: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(so, msgs[1], strlen(msgs[1]) + 1);
    // syscall_read(me, buffer, 99);
    // printf("SO: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(so, msgs[2], strlen(msgs[2]) + 1);
    // syscall_read(me, buffer, 99);
    // printf("SO: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(so, msgs[3], strlen(msgs[3]) + 1);
    closePipe("StudentToSO");
    closePipe("SOToStudent");
    end();
}


void so()
{
    char* msgs[4] = {
            "Soy SO",
            "Tu TP",
            "Por que?",
            "Que bueno, anda el scheduler?"
    };
    char buffer[256] = {0};
    int me = openPipe("StudentToSO");
    int student = openPipe("SOToStudent");
    // sleep(randBound(1000, 5000));
    syscall_write(student, msgs[0], strlen(msgs[0]) + 1);
    // syscall_read(me, buffer, 99);
    // printf("Student: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(student, msgs[1], strlen(msgs[1]) + 1);
    // syscall_read(me,buffer,99);
    // printf("Student: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(student, msgs[2], strlen(msgs[2]) + 1);
    // syscall_read(me, buffer, 99);
    // printf("Student: %s\n", buffer);
    // sleep(randBound(1000, 5000));
    // syscall_write(student, msgs[3], strlen(msgs[3]) + 1);
    // syscall_read(me, buffer, 99);
    // printf("Student: %s\n", buffer);
    closePipe("StudentToSO");
    closePipe("SOToStudent");
    end();
}
