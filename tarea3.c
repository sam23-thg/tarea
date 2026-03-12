#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BARRA 20

void barra(int progreso) {
    int llenos = progreso * BARRA / 100;

    for(int i=0;i<llenos;i++)
        printf("§");

    for(int i=llenos;i<BARRA;i++)
        printf(" ");

    printf(" %d%%", progreso);
}

int main() {

    int progreso = 0;
    char tecla;
    pid_t pid;

    while(progreso <= 100) {

        printf("\rProceso Padre ID %d: ", getpid());
        barra(progreso);
        fflush(stdout);

        usleep(300000);

        printf("\nPresiona h para crear hijo o cualquier tecla para continuar: ");
        scanf(" %c", &tecla);

        if(tecla == 'h') {

            pid = fork();

            if(pid == 0) { // HIJO

                for(int i=0;i<=100;i+=5) {
                    printf("\rProceso Hijo ID %d: ", getpid());
                    barra(i);
                    fflush(stdout);
                    usleep(200000);
                }

                printf("\n");
                exit(0);
            }
            else { // PADRE
                wait(NULL);
            }
        }

        progreso += 5;
    }

    printf("\nProceso padre terminado\n");

    return 0;
}
