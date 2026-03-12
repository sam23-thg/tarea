#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

volatile sig_atomic_t crear_hijo = 0;
pid_t hijo_pid = 0;

void manejador_sigint(int sig) {
    if (hijo_pid == 0) {
        crear_hijo = 1;
    }
}

void barra_progreso(const char *nombre, int pid) {

    for (int i = 0; i <= 100; i += 5) {

        int num = i * 20 / 100;

        printf("\r%s ID %d: ", nombre, pid);

        for (int j = 0; j < num; j++)
            printf("§");

        for (int j = num; j < 20; j++)
            printf(" ");

        printf("%3d%%", i);

        fflush(stdout);

        usleep(200000);
    }

    printf("\n");
}

int main() {

    struct sigaction sa;
    sa.sa_handler = manejador_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    int pid_padre = getpid();

    printf("Padre ejecutando. Presiona Ctrl+C para crear un hijo\n");

    for (int i = 0; i <= 100; i += 5) {

        if (hijo_pid == 0) {

            int num = i * 20 / 100;

            printf("\rProceso Padre ID %d: ", pid_padre);

            for (int j = 0; j < num; j++)
                printf("§");

            for (int j = num; j < 20; j++)
                printf(" ");

            printf("%3d%%", i);

            fflush(stdout);
        }

        if (crear_hijo) {

            crear_hijo = 0;

            hijo_pid = fork();

            if (hijo_pid == 0) {

                signal(SIGINT, SIG_DFL);

                printf("\nCreando proceso hijo...\n");

                barra_progreso("Proceso hijo", getpid());

                printf("Hijo terminado\n");

                exit(0);
            }

            else if (hijo_pid < 0) {

                perror("fork");
                hijo_pid = 0;
            }

            else {

                printf("\nHijo creado con PID %d\n", hijo_pid);

                fflush(stdout);
            }
        }

        usleep(300000);

        if (hijo_pid > 0) {

            int status;

            pid_t r = waitpid(hijo_pid, &status, WNOHANG);

            if (r == hijo_pid) {

                printf("\nHijo terminado. Padre continúa...\n");

                hijo_pid = 0;
            }
        }
    }

    if (hijo_pid > 0)
        waitpid(hijo_pid, NULL, 0);

    printf("\nProceso padre terminado\n");

    return 0;
}
