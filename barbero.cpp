#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

const int NUM_SILLAS = 5;

sem_t sem_sillas;
sem_t sem_barbero;
sem_t sem_cliente;
sem_t sem_corteTerminado;

void barber()
{
    while (1)
    {
        cout << "El barbero esta durmiendo.\n";
        sem_wait(&sem_cliente);

        cout << "El barbero esta atendiendo a un cliente.\n";
        this_thread::sleep_for(chrono::seconds(2)); // Simulación del tiempo de corte

        cout << "El corte ha terminado.\n";
        sem_post(&sem_corteTerminado);
        sem_post(&sem_barbero);
    }
}

void customer(int id)
{
    if (sem_trywait(&sem_sillas) == 0)
    {
        cout << "Cliente " << id << " se sienta en una silla de espera.\n";
        sem_post(&sem_cliente);
        sem_wait(&sem_barbero);
        cout << "Cliente " << id << " esta siendo atendido.\n";
        sem_wait(&sem_corteTerminado);
        sem_post(&sem_sillas);
        cout << "Cliente " << id << " se va satisfecho.\n";
    }
    else
        cout << "Cliente " << id << " no hay sillas disponibles, se va.\n";
}

int main()
{
    sem_init(&sem_sillas, 0, NUM_SILLAS);
    sem_init(&sem_barbero, 0, 0);
    sem_init(&sem_cliente, 0, 0);
    sem_init(&sem_corteTerminado, 0, 0);

    thread barberThread(barber);

    thread customerThread[10];
    for (int i = 0; i < 10; i++)
    {
        customerThread[i] = thread(customer, i);
        this_thread::sleep_for(chrono::seconds(1)); // Intervalo de llegada de los clientes
    }

    barberThread.join();
    for (int i = 0; i < 10; i++)
    {
        customerThread[i].join();
    }

    sem_destroy(&sem_sillas);
    sem_destroy(&sem_barbero);
    sem_destroy(&sem_cliente);
    sem_destroy(&sem_corteTerminado);
}