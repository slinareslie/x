#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

const int tb = 5;
int j = 0;
class buffer
{
public:
    void insertar(int i);
    void sacar(int i);

private:
    mutex mtxMonitor;
    condition_variable nolleno, novacio;
    int recurso[tb];
    int tam = 0, frente = 0, cola = 0;
} b;

void buffer::insertar(int i)
{
    unique_lock<mutex> lck(mtxMonitor);
    if (tam >= tb)
    {
        nolleno.wait(lck);
    }
    recurso[cola] = i;
    tam = tam + 1;
    cola = (cola + 1) % tb;
    novacio.notify_all();
}

void buffer::sacar(int i)
{
    unique_lock<mutex> lck(mtxMonitor);
    if (tam == 0)
    {
        novacio.wait(lck);
    }
    i = recurso[frente];
    tam = tam - 1;
    frente = (frente + 1) % tb;
    nolleno.notify_all();
}

void productor()
{

    while (true)
    {
        j = j + 1;

        b.insertar(j);
        cout << "Se produce el elemento " << j << endl;
    }
}

void consumidor()
{

    while (true)
    {
        b.sacar(j);
        cout << "Se consume el elemento " << j << endl;
    }
}

int main()
{
    thread producerThreads(productor);
    thread consumerThreads(consumidor);

    producerThreads.join();
    consumerThreads.join();
    return 0;
}