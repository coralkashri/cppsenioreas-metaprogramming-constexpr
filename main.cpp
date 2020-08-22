#include <iostream>
#include <chrono>
#include <vector>

namespace math {
    /* Factorial */
    template <size_t N>
    struct factorial {
        static constexpr auto val = N * factorial<N - 1>::val;
    };

    template <>
    struct factorial<1> { // Partial specialization will be discuss in a future post in this series
        static constexpr auto val = 1;
    };
}

#if __cplusplus <= 201103L // C++11

// Constexpr Variables

// Constexpr Functions

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
constexpr int factorial(T n) {
    return n <= 0 ? throw std::runtime_error("N must be positive") : (n * factorial(n - 1));
}

constexpr double pow(double base, int exponent) {
    return exponent == 0 ? 1 : exponent < 0 ?  pow(1.0 / base, -exponent) : (base * pow(base, exponent - 1));
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
constexpr int my_func(T base, T n) {
    return n <= 0 ? 1 : (-base / my_func(base, n - 1));
}

// Constexpr Objects

#elif __cplusplus > 201103L && __cplusplus <= 201402L // C++14

// Constexpr Varibles

template <size_t N>
constexpr auto factorial_v = math::factorial<N>::val;

// Constexpr Functions

// Constexpr Functions

template <typename T, typename = typename std::enable_if_t<std::is_integral<T>::value>>
constexpr auto factorial(T n) {
    auto res = n;

    if (n <= 0) throw std::runtime_error("N must be positive");

    while (--n) {
        res *= n;
    }
    return res;
}

constexpr double pow(double base, int exponent) {
    if (exponent < 0) {
        base = 1 / base;
        exponent *= -1;
    }

    double res = 1;

    while (exponent--) {
        res *= base;
    }

    return res;
}

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
constexpr auto my_func(T base, T n) {
    auto res = n <= 0 ? 1 : -base;
    while (--n > 0) {
        res /= -base;
    }
    return res;
}

// Constexpr Objects

#elif __cplusplus > 201402L && __cplusplus <= 201703L // C++17

#else // C++20

// Constexpr Functions

template <typename T>
concept Integral = std::is_integral_v<T>;

template <Integral T>
consteval auto factorial(T n) {
    T res = n;

    if (n <= 0) throw std::runtime_error("N must be positive");

    while (--n) {
        res *= n;
    }

    return res;
}

constexpr double pow(double base, int exponent) {
    if (exponent < 0) {
        base = 1 / base;
        exponent *= -1;
    }

    double res = 1;

    while (exponent--) {
        res *= base;
    }

    return res;
}


// Constexpr Objects

class date {
public:
    constexpr date(int day, int month, int year)
    : d(day), m(month), y(year) {
        self_balance();
    }

    [[nodiscard]] constexpr date offset(int days) const {
        return date(d + days, m, y);
    }

    [[nodiscard]] constexpr unsigned short get_day() const { return d; }
    [[nodiscard]] constexpr unsigned short get_month() const { return m; }
    [[nodiscard]] constexpr unsigned short get_year() const { return y; }

private:
    int d, m, y;

    constexpr void self_balance() {
        unsigned short days_in_month;
        unsigned short days_in_prev_month = 31;
        bool is_change_detected = false;
        if (m == 2) {
            if (!(y % 4)) {
                days_in_month = 29;
            } else {
                days_in_month = 28;
            }
        } else {
            if (m <= 7 && m % 2 || m >= 8 && m % 2 == 0) {
                days_in_month = 31;
                if (m != 8) {
                    days_in_prev_month = 30;
                }
            } else {
                days_in_month = 30;
            }
        }

        if (d > days_in_month) {
            d -= days_in_month;
            m++;
            is_change_detected = true;
        } else if (d < 1) {
            d += days_in_prev_month;
            m--;
            is_change_detected = true;
        }

        if (m > 12) {
            m = 1;
            y++;
            is_change_detected = true;
        } else if (m < 1) {
            m = 12 - m;
            y--;
            is_change_detected = true;
        }

        if (is_change_detected) self_balance();
    }
};

#endif

int main() {
    /*constexpr double n1 = 5e6;
    constexpr double n2 = 511;
    auto start = std::chrono::high_resolution_clock::now();
    auto res = my_func(n1, 511.0);
    //res = factorial(5);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Nano: " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
    std::cout << "Result: " << res << std::endl; // Make sure that optimization won't ignore the calculation*/

    static_assert(pow(2, 2) == 4, "");
    static_assert(pow(2, -2) == 0.25, "");
    static_assert(pow(2, 3) == 8, "");
    static_assert(pow(2, 4) == 16, "");
    static_assert(pow(4, 2) == 16, "");
    static_assert(pow(4, -2) == 0.0625, "");
    static_assert(pow(1, 0) == 1, "");
    static_assert(pow(500, 0) == 1, "");
    static_assert(factorial(5) == 120, "");
    static_assert(factorial(4) == 24, "");
    static_assert(factorial(3) == 6, "");
    static_assert(factorial(2) == 2, "");

    constexpr date d(23, 8, 2020);
    constexpr date d2 = d.offset(365);
    constexpr date d3 = d.offset(-365);
    std::cout << d2.get_day() << " / " << d2.get_month() << " / " << d2.get_year() << std::endl;
    std::cout << d3.get_day() << " / " << d3.get_month() << " / " << d3.get_year() << std::endl;

    return EXIT_SUCCESS;
}