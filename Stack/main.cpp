#include <iostream>
#include <stdexcept>

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


void testStavi() {
    std::cout << "Ocekivani izlaz: 3" << std::endl 
    << "Dobiveni izlaz: ";
    Stek<int> s;
    for (int i = 1; i <= 3; i++) s.stavi(i);
    std::cout << s.vrh() << std::endl;
}

void testObrisi() {
    std::cout << "Ocekivani izlaz: 3 0" << std::endl 
    << "Dobiveni izlaz: ";
    Stek<int> s;
    for (int i = 1; i <= 3; i++) s.stavi(i);
    std::cout << s.brojElemenata() << " ";
    s.brisi();
    std::cout << s.brojElemenata() << std::endl;
}

void testSkini() {
    std::cout << "Ocekivani izlaz: 3 2" << std::endl 
    << "Dobiveni izlaz: ";
    Stek<int> s;
    for (int i = 1; i <= 3; i++) s.stavi(i);
    std::cout << s.skini() << " " << s.vrh() << std::endl;
}

void testVrh() {
    std::cout << "Ocekivani izlaz: 3 10" << std::endl 
    << "Dobiveni izlaz: ";
    Stek<int> s;
    for (int i = 1; i <= 3; i++) s.stavi(i);
    std::cout << s.vrh() << " ";
    s.vrh() = 10;
    std::cout << s.vrh() << std::endl;
}

void testBrojElemenata() {
    std::cout << "Ocekivani izlaz: 0 3" << std::endl 
    << "Dobiveni izlaz: ";
    Stek<int> s;
    std::cout << s.brojElemenata() << " ";
    for (int i = 1; i <= 3; i++) s.stavi(i);
    std::cout << s.brojElemenata() << std::endl;
}

void testOpDodjeleKopKonst(){
    std::cout << "Ocekivani izlaz: 10 5 9 10" << std::endl 
    << "Dobiveni izlaz: ";
    Stek<int> s1;
    for (int i = 1; i <= 10; i++) s1.stavi(i);
    Stek<int> s2(s1);
    for (int i = 0; i < 5; i++) s2.skini();
    std::cout << s1.brojElemenata() << " " << s2.brojElemenata() << " ";
    s2 = s1;
    s1.skini();
    std::cout << s1.brojElemenata() << " " << s2.brojElemenata() << std::endl;
}


int main() {
    testStavi();
    testObrisi();
    testSkini();
    testVrh();
    testBrojElemenata();
    testOpDodjeleKopKonst();
    return 0;
}
