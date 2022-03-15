#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ID 1

struct msgSt
{
    long int mtype;
    int roll;
    char name[100];
};

struct msgHead
{
    long int mtype;
    int n;
};

int main()
{
    int msgid;
    if((msgid = msgget((key_t)1234, 0666| IPC_CREAT)) == -1)
    {
        perror("msgid");
        return(1);
    }

    printf("Enter number of students: ");
    struct msgHead header;
    scanf("%d", &header.n);

    struct msgSt stud[header.n];
    printf("Enter roll and name serially (Eg: 83 Nilay)\n");
    for(int i = 0; i < header.n; i++)
    {
        // stud[i].mtype = 2;
        scanf("%d%s", &stud[i].roll, stud[i].name);
    }

    for(int j = 2; j <= 3; j++)
    {
        int send = j;
        header.mtype = j;
        if (msgsnd(msgid, (void *)&header, sizeof(header), header.mtype) == -1)
        {
            perror("msgsnd");
            return(1);
        }

        for(int i = 0; i < header.n; i++)
        {
            stud[i].mtype = j;
            if (msgsnd(msgid, (void *)&stud[i], sizeof(stud[i]), stud[i].mtype) == -1)
            {
                perror("msgsnd");
                return(1);
            }
        }
    }
    printf("\nSorted by roll:\n");
    struct msgSt studRoll[header.n];

    for(int i = 0; i < header.n; i++)
    {
        if(msgrcv(msgid, (void *)&studRoll[i], sizeof(studRoll[i]), ID, 0) == -1)
        {
            perror("msgrcv");
            return(1);
        }
        printf("%s\t%d\n", studRoll[i].name, studRoll[i].roll);
    }
    printf("\nSorted by name:\n");
    struct msgSt studName[header.n];
    for(int i = 0; i < header.n; i++)
    {
        if(msgrcv(msgid, (void *)&studName[i], sizeof(studName[i]), ID, 0) == -1)
        {
            perror("msgrcv");
            return(1);
        }
        printf("%s\t%d\n", studName[i].name, studName[i].roll);
    }
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        perror("msgctl");
        return(1);
    }
    return 0;
}
