#include <syscalls.h>
#include <stdlib.h>
#include <prodCons.h>
#include <math.h>

int olvidameVersionSO(){
    void** parg = (void**)malloc(sizeof(void*) * 2);
    exec(&student, parg, "student");
    exec(&so, parg, "so");
}


int student(){
    char* msgs[4]={
            "¿Quien es?",
            "¿Que vienes a buscar?",
            "Ya es tarde",
            "Porque yo ya entregue el tp"
    };
    char buffer[100]={0};
    int me=openPipe("StudentToSO");
    int so=openPipe("SOToStudent");

    sleep(randBound(1000,5000));
    write(so,msgs[0],strlen(msgs[0])+1);
    read(me,buffer,99);
    printf("SO: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(so,msgs[1],strlen(msgs[1])+1);
    read(me,buffer,99);
    printf("SO: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(so,msgs[2],strlen(msgs[2])+1);
    read(me,buffer,99);
    printf("SO: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(so,msgs[3],strlen(msgs[3])+1);
    read(me,buffer,99);
    printf("SO: %s\n",buffer);
    closePipe("StudentToSO");
    closePipe("SOToStudent");

}


int joaquin(){
    char* msgs[4]={
            "Soy SO",
            "Tu TP",
            "¿Por que?",
            "Que bueno, ¿Anda el scheduler?"
    };
    char buffer[100]={0};
    int student=openPipe("StudentToSO");
    int me=openPipe("SOToStudent");
    read(me,buffer,99);
    printf("Student: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(student,msgs[0],strlen(msgs[0])+1);
    read(me,buffer,99);
    printf("Student: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(student,msgs[1],strlen(msgs[1])+1);
    read(me,buffer,99);
    printf("Student: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(student,msgs[2],strlen(msgs[2])+1);
    read(me,buffer,99);
    printf("Student: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(student,msgs[3],strlen(msgs[3])+1);
    closePipe("StudentToSO");
    closePipe("SOToStudent");


}
