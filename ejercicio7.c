#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BARRA 20

int progreso = 0;

void barra(int p){
    int llenos = p * BARRA / 100;

    for(int i=0;i<llenos;i++)
        printf("§");

    for(int i=llenos;i<BARRA;i++)
        printf(" ");

    printf(" %d%%", p);
}

void crear_hijo(int sig){

    pid_t pid = fork();

    if(pid == 0){   // HIJO

        printf("\nProceso hijo ID %d:\n", getpid());

        for(int i=0;i<=100;i+=5){
            printf("\rProceso hijo ID %d: ", getpid());
            barra(i);
            fflush(stdout);
            usleep(150000);
        }

        printf("\nProceso hijo terminado\n");
        exit(0);
    }
    else{
        wait(NULL); // padre espera al hijo
        printf("\nProceso Padre ID %d continua...\n", getpid());
    }
}

int main(){

    signal(SIGINT, crear_hijo);  // Ctrl+C crea un hijo

    printf("Proceso Padre ID %d\n", getpid());
    printf("Presiona Ctrl+C para crear un hijo\n\n");

    while(progreso <= 100){

        printf("\rProceso Padre ID %d: ", getpid());
        barra(progreso);
        fflush(stdout);

        progreso += 5;
        sleep(1);
    }

    printf("\nProceso padre terminado\n");

    return 0;
}
