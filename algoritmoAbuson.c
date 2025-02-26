#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>

#define MAX_RONDAS 10
#define FALLA_PROB 30 // Probabilidad de falla de un nodo en porcentaje

// Función que verifica si un nodo está fallando (probabilidad basada en FALLA_PROB)
bool nodo_fallando()
{
    int prob = rand() % 100;
    return prob < FALLA_PROB; // True si el nodo falla
}

// Función para generar un arreglo que indica qué nodos están fallando
int *generarNodosFallidos(int size)
{
    int *nodos_fallidos = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        nodos_fallidos[i] = nodo_fallando() ? 1 : 0;
    }
    return nodos_fallidos;
}

// Selección inicial de un líder basada en el rango más alto
int seleccionar_lider(int rank, int size, int *nodos_fallidos)
{
    for (int i = size - 1; i >= 0; i--)
    {
        if (!nodos_fallidos[i])
        {
            return i; // El nodo de mayor rango activo es el líder
        }
    }
    return -1; // Si todos los nodos fallan, no hay líder
}

// Proceso del algoritmo del abusón
void algoritmo_abuson(int rank, int size, int *nodos_fallidos)
{
    int lider_actual = seleccionar_lider(rank, size, nodos_fallidos);
    int consenso = 0; // Variable para determinar si se alcanzó un consenso

    if (rank == 0)
    {
        printf("El nodo inicial líder es: %d\n", lider_actual);
    }
    MPI_Bcast(&lider_actual, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Ciclo de rondas
    for (int ronda = 0; ronda < MAX_RONDAS && !consenso; ronda++)
    {
        if (nodos_fallidos[rank])
        {
            printf("Nodo %d falló en la ronda %d y no participa.\n", rank, ronda);
            continue;
        }

        printf("Nodo %d participa en la ronda %d con el líder actual: %d\n", rank, ronda, lider_actual);

        // Proceso de elección si el líder actual está fallando
        if (nodos_fallidos[lider_actual])
        {
            printf("Líder %d falló. Nodo %d inicia elección.\n", lider_actual, rank);
            int nuevo_lider = rank;

            // Notificar a todos los nodos de menor rango sobre la elección
            for (int i = rank + 1; i < size; i++)
            {
                if (!nodos_fallidos[i])
                {
                    MPI_Send(&rank, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }

            // Recibir respuestas de nodos con mayor rango
            for (int i = rank + 1; i < size; i++)
            {
                if (!nodos_fallidos[i])
                {
                    int respuesta;
                    MPI_Recv(&respuesta, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    if (respuesta > nuevo_lider)
                    {
                        nuevo_lider = respuesta;
                    }
                }
            }

            // Actualizar el líder
            lider_actual = nuevo_lider;
            printf("Nuevo líder elegido en la ronda %d: Nodo %d\n", ronda, lider_actual);
        }

        // Difundir el líder a todos los nodos
        MPI_Bcast(&lider_actual, 1, MPI_INT, lider_actual, MPI_COMM_WORLD);

        // Verificar consenso
        int acuerdo = 1;
        if (rank != lider_actual && !nodos_fallidos[rank])
        {
            acuerdo = (lider_actual >= 0 && !nodos_fallidos[lider_actual]) ? 1 : 0;
        }

        MPI_Allreduce(&acuerdo, &consenso, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);

        if (rank == 0)
        {
            if (consenso)
            {
                printf("Consenso alcanzado en la ronda %d: Nodo líder es %d\n", ronda, lider_actual);
            }
            else
            {
                printf("No se alcanzó el consenso en la ronda %d. Reiniciando elección.\n", ronda);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Semilla para aleatoriedad
    srand(time(NULL) + rank);

    // Generar nodos fallidos
    int *nodos_fallidos = generarNodosFallidos(size);
    if (rank == 0)
    {
        printf("Nodos fallidos: ");
        for (int i = 0; i < size; i++)
        {
            printf("%d ", nodos_fallidos[i]);
        }
        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Ejecutar algoritmo del abusón
    algoritmo_abuson(rank, size, nodos_fallidos);

    free(nodos_fallidos);
    MPI_Finalize();
    return 0;
}
