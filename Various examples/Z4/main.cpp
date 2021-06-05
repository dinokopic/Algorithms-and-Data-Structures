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

template <typename Tip>
Stek<Tip> rekurzijaSpojiSortirano(Stek<Tip> &s1, Stek<Tip> &s2) {
    Tip x;
    if (s1.brojElemenata() != 0 && (s2.brojElemenata() == 0 || s1.vrh() < s2.vrh()))
        x = s1.skini();
    else if (s2.brojElemenata() != 0)
        x = s2.skini();
    if (s1.brojElemenata() != 0 || s2.brojElemenata() != 0)
        rekurzijaSpojiSortirano(s1, s2);
    s1.stavi(x);
    return s1;
}

template <typename Tip>
Stek<Tip> spojiSortirano(Stek<Tip> s1, Stek<Tip> s2) {
    return rekurzijaSpojiSortirano(s1, s2);
}

int main() {
    Stek<int> prvi, drugi;
    for (int i = 10; i >= 1; i--)
        if (i%2 == 0) prvi.stavi(i);
        else drugi.stavi(i);
    Stek<int> novi(spojiSortirano(prvi,drugi));
    while (novi.brojElemenata()) {
        std::cout << novi.skini();
    }
    return 0;
}
