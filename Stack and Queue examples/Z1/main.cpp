#include <iostream>
#include <stdexcept>

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


void izbaciVeceOdSV(Red<int> &r) {
    int n = r.brojElemenata(), suma = 0;
    for (int i = 0; i < n; i++) {
        suma += r.celo();
        r.stavi(r.skini());
    }
    for (int i = 0; i < n; i++) {
        int broj = r.skini();
        if (broj <= double(suma)/n)
            r.stavi(broj);
    }
}


int main() {
    Red<int> r;
    r.stavi(1);
    r.stavi(2);
    r.stavi(3);
    r.stavi(4);
    r.stavi(5);
    izbaciVeceOdSV(r);
    for (int i = 0; i < r.brojElemenata(); i++) {
        std::cout << r.celo() << " ";
        r.stavi(r.skini());
    }
    
    return 0;
}
