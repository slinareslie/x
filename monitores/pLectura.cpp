#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

class PrioLec
{
public:
    void alectura();
    void clectura();
    void aescritura();
    void cescritura();

private:
    mutex mtxMonitor;
    condition_variable lector, escritor;
    int n1;
    bool escribiendo;
} pl;

void PrioLec::alectura()
{
    unique_lock<mutex> lck(mtxMonitor);
    if (escribiendo)
        lector.wait(lck);
    n1++;
    lector.notify_all();
}

void PrioLec::clectura()
{
    unique_lock<mutex> lck(mtxMonitor);
    n1--;
    if (n1 == 0)
        escritor.notify_all();
}

void PrioLec::aescritura()
{
    unique_lock<mutex> lck(mtxMonitor);
    if (n1 != 0 || escribiendo)
        escritor.wait(lck);
    escribiendo = true;
}

void PrioLec::cescritura()
{
    unique_lock<mutex> lck(mtxMonitor);
    escribiendo = false;
    escritor.notify_all();
    lector.notify_all();
}

void lector()
{
    for (int i = 0; i < 5; i++)
    {
        pl.alectura();
        cout << "Leer el recurso" << endl;
        pl.clectura();
    }
}

void escritor()
{
    for (int i = 0; i < 5; i++)
    {
        pl.aescritura();
        cout << "Escribir en el recurso" << endl;
        pl.cescritura();
    }
}

int main()
{
    thread lectorThreads[4];
    thread escritorThreads[4];

    for (int i = 0; i < 4; i++)
    {
        lectorThreads[i] = thread(lector);
        escritorThreads[i] = thread(escritor);
    }

    for (int i = 0; i < 4; i++)
    {
        lectorThreads[i].join();
        escritorThreads[i].join();
    }

    return 0;
}
