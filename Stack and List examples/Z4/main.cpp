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


int spoji(Stek<int> s1, Stek<int> s2, Stek<int> &s3) {
    while (s2.brojElemenata())
        s3.stavi(s2.skini());
    while (s1.brojElemenata() || s3.brojElemenata()) {
        if (s1.brojElemenata() && (!s3.brojElemenata() || s1.vrh() > s3.vrh())) s2.stavi(s1.skini());
        else s2.stavi(s3.skini());
    }
    s3.brisi();
    while (s2.brojElemenata())
        s3.stavi(s2.skini());
    return 0;
}

int main() {
    Stek<int> s1, s2, s3;
    for (int i = 1; i <= 10; i++) {
        if (i%2 == 0)  s1.stavi(i);
        else s2.stavi(10-i);
    }
    spoji(s1, s2, s3);
    while (s3.brojElemenata()) std::cout << s3.skini() << " ";
    return 0;
}
