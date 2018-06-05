#include <syscalls.h>
#include <stdlib.h>
#include <prodConsWithPipes.h>
#include <math.h>

void olvidameVersionSO()
{
    printf("Por favor espere a que termine.\n");
    void** parg = (void**)malloc(sizeof(void*) * 2);
    exec(&student, parg, "student");
    exec(&so, parg, "so");
}


void student()
{
    char* msgs[4] = {
            "Soy SO",
            "Tu TP",
            "Por que?",
            "Que bueno, anda el scheduler?"
    };
    char buffer[256] = {0};
    int me = openPipe("SOToStudent");
    int so = openPipe("StudentToSO");
    syscall_read(me, buffer, 99);

    printf("Student: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(so, msgs[0], strlen(msgs[0]) + 1);
    syscall_read(me, buffer, 99);

    printf("Student: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(so, msgs[1], strlen(msgs[1]) + 1);
    syscall_read(me, buffer, 99);

    printf("Student: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(so, msgs[2], strlen(msgs[2]) + 1);
    syscall_read(me, buffer, 99);

    printf("Student: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(so, msgs[3], strlen(msgs[3]) + 1);
    closePipe("StudentToSO");
    closePipe("SOToStudent");
    end();
}


void so()
{
    char* msgs[4] = {
            "Quien es?",
            "Que vienes a buscar?",
            "Ya es tarde",
            "Porque yo ya entregue el tp"
    };
    char buffer[256] = {0};
    int student = openPipe("SOToStudent");
    int me = openPipe("StudentToSO");
    sleep(randBound(1000, 3000));
    syscall_write(student, msgs[0], strlen(msgs[0]) + 1);
    syscall_read(me, buffer, 99);

    printf("SO: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(student, msgs[1], strlen(msgs[1]) + 1);
    syscall_read(me,buffer,99);

    printf("SO: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(student, msgs[2], strlen(msgs[2]) + 1);
    syscall_read(me, buffer, 99);

    printf("SO: %s\n", buffer);
    sleep(randBound(1000, 3000));
    syscall_write(student, msgs[3], strlen(msgs[3]) + 1);
    syscall_read(me, buffer, 99);

    printf("SO: %s\n", buffer);
    printf("Fin\n");
    closePipe("StudentToSO");
    closePipe("SOToStudent");
    end();
}
