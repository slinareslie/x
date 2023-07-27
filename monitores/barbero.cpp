#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

class BarberoDormilon
{
public:
    void cortarPelo(int clienteId);
    void siguienteCliente();

private:
    mutex mtxMonitor;
    condition_variable esperarCliente, esperarBarbero;
    bool barberoDisponible = true;
    int numClientesEsperando = 0;
};

void BarberoDormilon::cortarPelo(int clienteId)
{
    unique_lock<mutex> lck(mtxMonitor);
    if (!barberoDisponible)
    {
        cout << "Cliente " << clienteId << " esperando." << endl;
        numClientesEsperando++;
        esperarCliente.wait(lck);
    }
    cout << "Barbero cortando el pelo al cliente " << clienteId << endl;
    barberoDisponible = false;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Barbero termino de cortar el pelo al cliente " << clienteId << endl;
    barberoDisponible = true;
    esperarBarbero.notify_one();
}

void BarberoDormilon::siguienteCliente()
{
    unique_lock<mutex> lck(mtxMonitor);
    if (numClientesEsperando == 0)
    {
        cout << "Barbero durmiendo." << endl;
        esperarBarbero.wait(lck);
    }
    numClientesEsperando--;
    esperarCliente.notify_one();
}

BarberoDormilon bd;

void cliente(int id)
{
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(250));
        bd.cortarPelo(id);
    }
}

void barbero()
{
    while (true)
    {
        bd.siguienteCliente();
    }
}

int main()
{
    thread barberThread(barbero);

    thread clientThreads[5];
    for (int i = 0; i < 5; i++)
    {
        clientThreads[i] = thread(cliente, i + 1);
    }

    for (int i = 0; i < 5; i++)
    {
        clientThreads[i].join();
    }

    return 0;
}
