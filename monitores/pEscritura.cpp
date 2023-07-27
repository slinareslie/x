#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

class le_monitor
{
public:
    void abrirLectura();
    void cerrarLectura();
    void abrirEscritura();
    void cerrarEscritura();

private:
    mutex mtxMonitor;
    condition_variable lector, escritor;
    int nl = 0;
    bool escribiendo = false;
};

void le_monitor::abrirLectura()
{
    unique_lock<mutex> lck(mtxMonitor);
    if (escribiendo || nl != 0)
    {
        lector.wait(lck);
    }
    nl++;
}

void le_monitor::cerrarLectura()
{
    unique_lock<mutex> lck(mtxMonitor);
    nl--;
    if (nl == 0)
    {
        escritor.notify_one();
    }
}

void le_monitor::abrirEscritura()
{
    unique_lock<mutex> lck(mtxMonitor);
    if (nl != 0 || escribiendo)
    {
        escritor.wait(lck);
    }
    escribiendo = true;
}

void le_monitor::cerrarEscritura()
{
    unique_lock<mutex> lck(mtxMonitor);
    escribiendo = false;
    if (nl != 0)
    {
        escritor.notify_one();
    }
    else
    {

        lector.notify_one();
    }
}

le_monitor le;

void lector()
{
    for (int i = 0; i < 5; i++)
    {
        le.abrirLectura();
        cout << "Lector" << endl;
        le.cerrarLectura();
    }
}

void escritor()
{
    for (int i = 0; i < 5; i++)
    {
        le.abrirEscritura();
        cout << "Escritor" << endl;
        le.cerrarEscritura();
    }
}

int main()
{
    thread l[3], e[3];

    for (int i = 0; i < 3; i++)
    {

        e[i] = thread(escritor);
        l[i] = thread(lector);
    }

    for (int i = 0; i < 5; i++)
    {
        l[i].join();
        e[i].join();
    }

    return 0;
}
