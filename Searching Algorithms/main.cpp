#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename Tip>
class Stek {
    struct Cvor {
        Tip el;
        Cvor *sljedeci;
    };
    Cvor *top;
    int brEl;
public:
    Stek() : top(nullptr), brEl(0) {}
    ~Stek() { brisi(); }
    Stek(const Stek &s);
    Stek &operator =(const Stek &s);
    void brisi();
    void stavi(const Tip &el);
    Tip skini();
    Tip &vrh() const { 
        if (!brEl) throw std::domain_error("Prazan stek");
        return top->el; 
    }
    int brojElemenata() const { return brEl; }
};

template <typename Tip>
void Stek<Tip>::brisi() {
    Cvor *pok = top;
    while (top) {
        top = top->sljedeci;
        delete pok;
        pok = top;
    }
    brEl = 0;
}

template <typename Tip>
Tip Stek<Tip>::skini() {
    if (!brEl) throw std::domain_error("Prazan stek");
    Tip element = top->el;
    auto pok = top;
    top = top->sljedeci;
    delete pok;
    brEl--;
    return element;
}

template <typename Tip>
void Stek<Tip>::stavi(const Tip &el) {
    Cvor *novi = new Cvor;
    novi->el = el;
    novi->sljedeci = top;
    top = novi;
    brEl++;
}

template <typename Tip>
Stek<Tip>::Stek(const Stek<Tip> &s) : top(nullptr), brEl(0) {
    Cvor *pok1 = s.top, *pok2(nullptr);
    while (pok1) {
        if (!top) {
            top = new Cvor;
            pok2 = top;
            top->sljedeci = nullptr;
        }
        else {
            top->sljedeci = new Cvor;
            top = top->sljedeci;
        }
        top->el = pok1->el;
        pok1 = pok1->sljedeci;
    }
    top->sljedeci = nullptr;
    top = pok2;
    brEl = s.brEl;
}

template <typename Tip>
Stek<Tip> &Stek<Tip>::operator =(const Stek<Tip> &s) {
    if (this == &s) return *this;
    brisi();
    Cvor *pok1 = s.top, *pok2(nullptr);
    while (pok1) {
        if (!top) {
            top = new Cvor;
            pok2 = top;
            top->sljedeci = nullptr;
        }
        else {
            top->sljedeci = new Cvor;
            top = top->sljedeci;
        }
        top->el = pok1->el;
        pok1 = pok1->sljedeci;
    }
    top->sljedeci = nullptr;
    top = pok2;
    brEl = s.brEl;
    return *this;
}

int binarnaPretraga(const std::vector<int> &v, int trazeni) {
    int pocetak = 0, kraj = v.size()-1, sredina;
    while (kraj >= pocetak) {
        sredina = (pocetak + kraj)/2;
        if (v[sredina] == trazeni)
            return sredina;
        else if (v[sredina] < trazeni) 
            pocetak = sredina + 1;
        else 
            kraj = sredina - 1;
    }
    return -1;
}


void pretraga(Stek<std::vector<int>> &s, int trazeni) {
    if (!s.brojElemenata()){
        std::cout << "Nema elementa";
        return;
    }
    std::vector<int> v(s.skini());
    if (v.size() != 0 && v[0] <= trazeni && v[v.size()-1] >= trazeni){
        int indeks = binarnaPretraga(v, trazeni);
        if (indeks == -1) 
            pretraga(s, trazeni);
        else  
            std::cout << indeks << " " << s.brojElemenata();
    }
    else if (s.brojElemenata() == 0) {
        std::cout << "Nema elementa";
    }
    else pretraga(s, trazeni);
    s.stavi(v);
}

void test1() {
    std::cout << "Ocekivani rezultat: 2 0" << std::endl;
    std::cout << "Dobiveni rezultat: ";
    std::vector<int> v{1,2,3,4,5};
    Stek<std::vector<int>> s;
    s.stavi(v);
    pretraga(s,3);
}

void test2() {
    std::cout << "Ocekivani rezultat: 1 1" << std::endl;
    std::cout << "Dobiveni rezultat: ";
    std::vector<int> v1{1,2,3,4,5}, v2{6,7}, v3{}, v4{8,9,10}, v5{11,12};
    Stek<std::vector<int>> s;
    s.stavi(v1);
    s.stavi(v2);
    s.stavi(v3);
    s.stavi(v4);
    s.stavi(v5);
    pretraga(s,7);
}

void test3() {
    std::cout << "Ocekivani rezultat: Nema elementa" << std::endl;
    std::cout << "Dobiveni rezultat: ";
    std::vector<int> v1{1,2,3,4,5}, v2{6,7}, v3{}, v4{8,9,10}, v5{11,12};
    Stek<std::vector<int>> s;
    s.stavi(v1);
    s.stavi(v2);
    s.stavi(v3);
    s.stavi(v4);
    s.stavi(v5);
    pretraga(s,13);
}


int main() {
    test3();
    return 0;
}
