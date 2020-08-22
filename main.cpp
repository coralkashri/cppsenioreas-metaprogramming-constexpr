#include <iostream>
#include <chrono>


#if __cplusplus <= 201103L // C++11

template <typename T>
constexpr double sum(std::initializer_list<T> &items) {
    return res;
}

#elif __cplusplus > 201103L && __cplusplus <= 201402L // C++14

#elif __cplusplus > 201402L && __cplusplus <= 201703L // C++17

#else // C++20

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic ...Args>
constexpr auto sum(Args ...args) {
    auto res = (args / ...);
    for (size_t i = 0; i < 262144; i++) {
        res /= (args / ...);
    }
    return res;
}

#endif

int main() {
    int a = 90;
    auto start = std::chrono::high_resolution_clock::now();
    //double res = sum(a, 200.3, 8, 4, 5, 3.2, 1, 0.2, 200);
    constexpr double res = sum(90.0, 200.3, 8, 4, 5, 3.2, 1, 0.2, 200);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
    std::cout << res << std::endl;
    return 0;
}
