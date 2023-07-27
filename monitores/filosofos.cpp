#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

const int N = 5;
enum Estado
{
    pensando,
    comiendo,
    hambriento
};

class ComidaFilosofos
{
public:
    void cogerPalillos(int i);
    void soltarPalillos(int i);

private:
    mutex mtxMonitor;
    condition_variable dormir[N];
    Estado estado[N];
    void test(int k);
} cf;

void ComidaFilosofos::test(int k)
{
    if (estado[(k + N - 1) % N] != comiendo && estado[(k + 1) % N] != comiendo)
    {
        estado[k] = comiendo;
        dormir[k].notify_all();
    }
}

void ComidaFilosofos::cogerPalillos(int i)
{
    estado[i] = hambriento;
    test(i);
    if (estado[i] != comiendo)
    {
        unique_lock<mutex> lck(mtxMonitor);
        dormir[i].wait(lck);
    }
}

void ComidaFilosofos::soltarPalillos(int i)
{
    estado[i] = pensando;
    test((i + 4) % N);
    test((i + 1) % N);
}

void filosofo(int i)
{
    while (true)
    {
        cout << "El filosofo " << i + 1 << " esta pensando" << endl;
        cf.cogerPalillos(i);
        cout << "El filosofo " << i + 1 << " esta comiendo" << endl;
        cf.soltarPalillos(i);
        cout << "El filosofo " << i + 1 << " termino de comer" << endl;
    }
}

int main()
{
    thread filosofosThreads[N];

    for (int i = 0; i < N; i++)
    {
        filosofosThreads[i] = thread(filosofo, i);
    }

    for (int i = 0; i < N; i++)
    {
        filosofosThreads[i].join();
    }

    return 0;
}
