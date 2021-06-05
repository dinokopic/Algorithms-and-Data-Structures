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
class Red {
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
    };
    Cvor *pocetak, *kraj;
    int brEl;
    void obrisiIPomjeri() {
        Cvor *pok = pocetak;
        pocetak = pocetak->sljedeci;
        delete pok;
        brEl--;
    }
public:
    Red() : pocetak(nullptr), kraj(nullptr), brEl(0) {}
    ~Red() { brisi(); }
    Red(const Red &r);
    void brisi() {
        while (brEl != 0) {
            obrisiIPomjeri();
        }
        pocetak = nullptr; kraj = nullptr;
    }
    void stavi(const Tip &el);
    Tip skini() {
        if (brEl == 0) throw std::logic_error("Izuzetak");
        Tip celo = pocetak->element;
        obrisiIPomjeri();
        return celo;
    }
    Tip &celo() { return pocetak->element; }
    int brojElemenata() const { return brEl; }
    Red &operator =(const Red &r);
};

template <typename Tip>
void Red<Tip>::stavi(const Tip &el) {
    Cvor *pok = new Cvor();
        pok->sljedeci = nullptr; pok->element = el;
        if (brEl == 0) {
            pocetak = pok;
            kraj = pok;
        }
        else {
            kraj->sljedeci = pok;
            kraj = pok;
        }
        brEl++;
}

template <typename Tip>
Red<Tip>::Red(const Red<Tip> &r) : brEl(0) {
    Cvor *pok = r.pocetak;
    while (pok) {
        this->stavi(pok->element);
        pok = pok->sljedeci;
    }
}

template <typename Tip>
Red<Tip> &Red<Tip>::operator =(const Red<Tip> &r) {
    if (this == &r) return *this;
    brisi();
    Cvor *pok = r.pocetak;
    while (pok) {
        this->stavi(pok->element);
        pok = pok->sljedeci;
    }
    return *this;
}


void prebaci(Red<int> &r, Stek<int> &s, int pocetnaDuzinaReda) {
    while(s.brojElemenata()) {
        r.stavi(s.skini());
    }
    int pocetnaDuzinaSteka = r.brojElemenata() - pocetnaDuzinaReda;
    while (r.brojElemenata()) {
        s.stavi(r.skini());
    }
    for (int i = 0; i < pocetnaDuzinaSteka; i++) {
        r.stavi(s.skini());
    }
    while (r.brojElemenata()) {
        s.stavi(r.skini());
    }
}


int main() {
    Red<int> r;
    for(int i = 1; i <= 5; i++) r.stavi(i);
    Stek<int> s;
    for (int i = 6; i <= 10; i++) s.stavi(i);
    
    prebaci(r,s,r.brojElemenata());
    while (s.brojElemenata()) {
        std::cout << s.skini() << " ";
    }
    
    return 0;
}
