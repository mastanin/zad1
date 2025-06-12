#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>


struct Kandidat {
    std::string ime;
    int glasovi = 0;

    
    friend std::istream& operator>>(std::istream& is, Kandidat& k) {
        is >> std::quoted(k.ime) >> k.glasovi;
        return is;
    }

    // operator za ispis
    friend std::ostream& operator<<(std::ostream& os, const Kandidat& k) {
        os << std::left << std::setw(30) << k.ime
            << " | Glasova: " << k.glasovi;
        return os;
    }
};


template <typename T>
class IzboriLok25 {
protected:
    std::vector<T> kandidati;

public:
    void dodajKandidata(const T& k) {
        kandidati.push_back(k);
    }

    friend std::ostream& operator<<(std::ostream& os, const IzboriLok25<T>& izbor) {
        for (const auto& k : izbor.kandidati) {
            os << k << std::endl;
        }
        return os;
    }
};


class NacelnikMajur25 : public IzboriLok25<Kandidat> {};
class OpcinskoVijeceMajur25 : public IzboriLok25<Kandidat> {};


bool jeBroj(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) {
        return std::isdigit(c) || c == '-';
        });
}

std::vector<Kandidat> ucitajKandidateIzCSV(const std::string& nazivDatoteke) {
    std::ifstream file(nazivDatoteke);
    std::vector<Kandidat> kandidati;

    if (!file.is_open()) {
        std::cerr << "Greška: ne mogu otvoriti " << nazivDatoteke << "\n";
        return kandidati;
    }

    std::string line;

    // Preskoči prva dva reda
    std::getline(file, line);
    std::getline(file, line);

    
    std::getline(file, line); // zaglavlja ignoriramo

    
    std::vector<std::string> imena = {
        "GORAN BUNJEVAC",
        "KLEMENTINA KARANOVIĆ"
    };

    std::vector<int> glasovi(imena.size(), 0);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string kolona;
        std::vector<std::string> polja;
        while (std::getline(ss, kolona, ';')) {
            polja.push_back(kolona);
        }

        // Očekujemo da su zadnje 2 kolone glasovi
        if (polja.size() >= 15) {
            try {
                glasovi[0] += std::stoi(polja[13]);
                glasovi[1] += std::stoi(polja[14]);
            } catch (...) {
                continue;
            }
        }
    }

    for (size_t i = 0; i < imena.size(); ++i) {
        kandidati.push_back({imena[i], glasovi[i]});
    }

    return kandidati;
}


int main() {
    NacelnikMajur25 nacelnik;
    OpcinskoVijeceMajur25 vijece;

    
    for (const auto& k : ucitajKandidateIzCSV("557_03_MAJUR_nacelnik.csv")) {
        nacelnik.dodajKandidata(k);
    }

    for (const auto& k : ucitajKandidateIzCSV("557_03_MAJUR_opcinsko.csv")) {
        vijece.dodajKandidata(k);
    }

    
    std::cout << "=== IZBORI ZA NAČELNIKA - MAJUR 2025 ===\n";
    std::cout << nacelnik;

    std::cout << "\n=== IZBORI ZA OPĆINSKO VIJEĆE - MAJUR 2025 ===\n";
    std::cout << vijece;

    return 0;
}
