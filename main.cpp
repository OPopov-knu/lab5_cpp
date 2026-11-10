#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <syncstream>
#include <string>

using namespace std;

// Варіант 23
// Усі обчислення: A1, A2, B, C1, C2, D, F
// "Повільні" обчислення: A2, D
// Залежності: B(A1, A2), C1(B), C2(B, D), F(D)
//
// Схема виконання (9 секунд, 3 потоки: main + 2 запуски async):
//   main:     A2 [0-7]  B [7-8]  C1 [8-9]
//   async #1: A1 [0-1]
//   async #2: D  [0-7]  F [7-8]  C2 [8-9]
// Точки синхронізації:
//   main чекає завершення async #1 (A1) перед B
//   async #2 чекає promise від main (B виконано) перед C2
//   main чекає завершення async #2 перед виведенням Work is done!

void slow(const string& name)
{
    this_thread::sleep_for(7s);
    osyncstream(cout) << name << "\n";
}

void quick(const string& name)
{
    this_thread::sleep_for(1s);
    osyncstream(cout) << name << "\n";
}

void work()
{
    auto start_time = chrono::steady_clock::now();

    promise<void> b_done;
    future<void> b_ready = b_done.get_future();

    future<void> t1 = async(launch::async, []() {
        quick("A1");
    });

    future<void> t2 = async(launch::async, [&]() {
        slow("D");
        quick("F");
        b_ready.wait();
        quick("C2");
    });

    slow("A2");
    t1.wait();
    quick("B");
    b_done.set_value();
    quick("C1");
    t2.wait();

    chrono::duration<double> d = chrono::steady_clock::now() - start_time;
    osyncstream(cout) << "Час виконання: " << d.count() << " с\n";
    osyncstream(cout) << "Work is done!\n";
}

int main()
{
    work();
}
