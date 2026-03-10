#include <cstddef>
#include <vector>
#include "bigdb.hpp"
#include <random>
#include <iostream>
#include <thread>

//benchmark code entirely stolen from https://gist.github.com/abcdabcd987/53b7aa6fdb8f7dbe46798fa6df2f5871
//slightly repurposed for generics, I guess.
std::string random_string(int len) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(123);
    static std::uniform_int_distribution<int> dist(0, sizeof(alphanum) - 1);
    std::string s;
    for (int i = 0; i < len; ++i)
        s += alphanum[dist(gen)];
    return s;
}

template<class T>
void do_worker(size_t seed, T& kv, const std::vector<std::string>& key_set, size_t num_ops) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist_choice(1, 100), dist_key(0, key_set.size()-1), dist_value;
    for (size_t i = 0; i < num_ops; ++i) {
        auto choice = dist_choice(gen);
        auto &key = key_set[dist_key(gen)];
        if (choice <= 33)
            kv.put(key, dist_value(gen));
        else if (choice <= 66)
            (void) kv.get(key);
        else
            (void) kv.remove(key);
    }
}

template<class T, typename ...Args>
void run(size_t num_workers, size_t num_keys, size_t num_ops, const std::vector<std::string> &key_set, Args&& ...args) {
    for (size_t i = 0; i < 16; ++i) {
        T kv(std::forward<Args>(args)...);

        std::vector<std::thread> workers;
        for (size_t i = 0; i < num_workers; ++i)
            workers.emplace_back(do_worker<T>, i+1, std::ref(kv), std::ref(key_set), num_ops);

        auto st = std::chrono::high_resolution_clock::now();
        for (auto &worker : workers)
            worker.join();
        auto ed = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = ed-st;
        double time = diff.count();

        std::cout << time << ' ' << std::flush;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    const size_t num_workers = std::stoi(argv[1]);
    const size_t num_keys = 1000000;
    const size_t num_ops = 10000000;

    std::vector<std::string> key_set;
    for (size_t i = 0; i < num_keys; ++i)
        key_set.emplace_back(random_string(32));

    if (strcmp(argv[2], "kvbiglock") == 0)
        run<bigdblock<std::string, int>>(num_workers, num_keys, num_ops, key_set);
    else if (strcmp(argv[2], "bigdbsharded") == 0)
        run<bigdbsharded<std::string, int>>(num_workers, num_keys, num_ops, key_set, std::stoi(argv[3]));
    else
        return 1;
}