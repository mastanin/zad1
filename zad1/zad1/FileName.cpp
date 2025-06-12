#include <vector>
#include <numeric>
#include <iostream>
#include <functional>
#include <ctime>
#include <thread>
#include <cstdlib>

template<typename T>
class SrednjaVrijednostVector : public std::vector<T> {
    size_t N;

public:
    SrednjaVrijednostVector(size_t n) : N(n) {}

    void dodaj(const T& value) {
        this->push_back(value);
    }

    std::function<void(size_t)> ispisiSrednjuVrijednost = [this](size_t start) {
        if (start + N > this->size()) {
            std::cout << "Nema dovoljno podataka za srednju vrijednost.\n";
            return;
        }

        T suma = std::accumulate(this->begin() + start, this->begin() + start + N, T{});
        std::cout << "Srednja vrijednost od pozicije " << start << " je: "
            << static_cast<double>(suma) / N << "\n";
        };
};
int main() {
    SrednjaVrijednostVector<int> brojevi(5);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int i = 0; i < 20; ++i) {
        int novi = std::rand() % 100;
        brojevi.dodaj(novi);
        std::cout << "Dodano: " << novi << "\n";
        if (brojevi.size() >= 5) {
            brojevi.ispisiSrednjuVrijednost(brojevi.size() - 5);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}