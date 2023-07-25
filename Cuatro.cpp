#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
using namespace std;

const int noFilosofos = 7;

struct Semaphore
{
    unique_ptr<mutex> mtx;
};

void filosofopar(int i, Semaphore *palillo)
{
    while (true)
    {
        cout << i << " : Esta leyendo" << endl;
        palillo[(i + 1) % noFilosofos].mtx->lock();
        palillo[i].mtx->lock();
        cout << i << " : Come" << endl;
        this_thread::sleep_for(chrono::milliseconds(1500));
        cout << i << " : Termina de comer" << endl;
        palillo[i].mtx->unlock();
        palillo[(i + 1) % noFilosofos].mtx->unlock();
    }
}

void filosofoimpar(int i, Semaphore *palillo)
{
    while (true)
    {
        cout << i << " : Esta leyendo" << endl;
        palillo[i].mtx->lock();
        palillo[(i + 1) % noFilosofos].mtx->lock();
        cout << i << " : Come" << endl;
        this_thread::sleep_for(chrono::milliseconds(1500));
        cout << i << " : Termina de comer" << endl;
        palillo[(i + 1) % noFilosofos].mtx->unlock();
        palillo[i].mtx->unlock();
    }
}

int main()
{
    Semaphore palillo[noFilosofos];
    thread filosofos[noFilosofos];

    for (int k = 0; k < noFilosofos; ++k)
    {
        palillo[k].mtx = make_unique<mutex>();
    }

    for (int k = 0; k < noFilosofos; ++k)
    {
        if (k == 0 || k == 1)
        {
            filosofos[k] = thread(filosofopar, k, palillo);
        }
        else
        {
            filosofos[k] = thread(filosofoimpar, k, palillo);
        }
    }

    for (auto &filosofo : filosofos)
    {
        filosofo.join();
    }

    return 0;
}
