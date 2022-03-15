#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ID 2
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

void sort(struct msgSt arr[], int n);
void swap(struct msgSt *a, struct msgSt *b);

int main()
{
    int msgid;
    if((msgid = msgget((key_t)1234, 0666| IPC_CREAT)) == -1)
    {
        perror("msgid");
        return(1);
    }

    struct msgHead header;
    if(msgrcv(msgid, (void *)&header, sizeof(header), ID, 0) == -1)
    {
        perror("msgrcv");
        return(1);
    }

    printf("Number of students: %d\n", header.n);
    struct msgSt stud[header.n];
    printf("Received data:\n");
    for(int i = 0; i < header.n; i++)
    {
        if(msgrcv(msgid, (void *)&stud[i], sizeof(stud[i]), ID, 0) == -1)
        {
            perror("msgrcv");
            return(1);
        }
        printf("%s\t%d\n", stud[i].name, stud[i].roll);
        stud[i].mtype = 1;
    }

    sort(stud, header.n);
    for(int i = 0; i < header.n; i++)
    {
        // stud[i].mtype = 0;
        if (msgsnd(msgid, (void *)&stud[i], sizeof(stud[i]), stud[i].mtype) == -1)
        {
            perror("msgsnd");
            return(1);
        }
    }
    printf("Roll wise sorted data sent.\n");
    return 0;
}

void sort(struct msgSt arr[], int n)
{
    for(int i = 0, flag = 1; i < n-1 && flag; i++)
    {
        flag = 0;
        for(int j = 0; j < n-i-1; j++)
        {
            if(arr[j].roll > arr[j+1].roll)
            {
                swap(&arr[j], &arr[j+1]);
                flag = 1;
            }
        }
    }
}

void swap(struct msgSt *a, struct msgSt *b)
{
    struct msgSt temp;
    temp.roll = a->roll;
    a->roll = b->roll;
    b->roll = temp.roll;

    strcpy(temp.name, a->name);
    strcpy(a->name, b->name);
    strcpy(b->name, temp.name);
}