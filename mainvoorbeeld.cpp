#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.h"

// dit is voor de threading met een threadpool
// zie voor referentie bij threadpool.cpp

//gebruik:
// maake een threadpool met 4 threads
//ThreadPool pool(4);

// queue en sla iets op
//auto result = pool.enqueue([](int answer) { return answer; }, 42);

// haal iets op
//std::cout << result.get() << std::endl;





int main()
{
    
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    
    return 0;
}
