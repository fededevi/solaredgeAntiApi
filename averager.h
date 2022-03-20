#pragma once

#include <mutex>

class Averager
{
public:
    void add(double value) {
        mutex.lock();
        count ++;
        sum += value;
        mutex.unlock();
    };

    double get(){
        if (sum == 0) return 0;
        mutex.lock();
        double val = sum / (double)count;
        count = 0;
        sum = 0;
        mutex.unlock();

        return val;
    };
private:
    double sum = 0;
    int count = 0;

    std::mutex mutex;


};

