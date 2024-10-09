


#include "kmp_log.hpp"

#include <ranges>
#include <vector>



auto foo() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v
    | std::views::transform([](int x) { return x * x; })
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::reverse;

}

int main() {
    kmp::Log::i("test {}", 1234);
}
