#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#include <fcntl.h>

#define BARRA 20

// Detectar tecla sin bloquear
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void barra(int progreso){
    int llenos = progreso * BARRA / 100;

    for(int i=0;i<llenos;i++)
        printf("§");
    for(int i=llenos;i<BARRA;i++)
        printf(" ");

    printf("%d%%", progreso);
}

int main(){

    int progreso = 0;
    pid_t pid;

    printf("Proceso Padre ID %d: ", getpid());

    while(progreso <= 100){

        if(kbhit()){
            char c = getchar();

            if(c == 'h'){
                printf("\n");

                pid = fork();

                if(pid == 0){ // HIJO

                    printf("Proceso hijo ID %d: ", getpid());

                    for(int i=0;i<=100;i+=5){
                        printf("\rProceso hijo ID %d: ", getpid());
                        barra(i);
                        fflush(stdout);
                        usleep(150000);
                    }

                    printf("\n");
                    exit(0);
                }
                else{ // PADRE
                    wait(NULL);
                    printf("Proceso Padre ID %d: ", getpid());
                }
            }
        }

        printf("\rProceso Padre ID %d: ", getpid());
        barra(progreso);
        fflush(stdout);

        progreso += 5;
        usleep(200000);
    }

    printf("\nProceso padre terminado\n");

    return 0;
}
