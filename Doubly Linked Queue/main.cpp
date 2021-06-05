#include <iostream>
#include <algorithm>
#include <stdexcept>

#define VELICINA_LISTE 1000

template <typename Tip>
class Iterator;

template <typename Tip>
class Lista{
public:
    Lista() = default;
    Lista(const Lista &l);
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual Tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip &el) = 0;
    virtual void dodajIza(const Tip &el) = 0;
    virtual Tip &operator [] (int i) = 0;
    virtual Tip operator [] (int i) const = 0;
    virtual ~Lista() {}
    friend class Iterator<Tip>;
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip el;
        Cvor *prethodni, *sljedeci;
    };
    Cvor *prvi, *posljednji, *t;
    int br_el;
    void alocirajKopiju(const DvostrukaLista &l);
public:
    DvostrukaLista() : prvi(nullptr), posljednji(nullptr), t(nullptr), br_el(0) {}
    DvostrukaLista(const DvostrukaLista &l) : br_el(l.br_el) {
        alocirajKopiju(l);    
    }
    ~DvostrukaLista() {
        while (prvi) {
            t = prvi;
            prvi = prvi->sljedeci;
            delete t;
        }
    }
    int brojElemenata() const override { return br_el; }
    Tip& trenutni() override {
        if(!br_el) throw "Lista je prazna";
        return t->el;
    }
    Tip trenutni() const override {
        if(!br_el) throw "Lista je prazna";
        return t->el;
    }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override { t = prvi; }
    void kraj() override { t = posljednji; }
    void obrisi() override;
    void dodajIspred(const Tip &el) override;
    void dodajIza(const Tip &el) override;
    Tip &operator [] (int i) override {
        if (i < 0 || i >= br_el) throw std::range_error("Indeks nije validan");
        Cvor *pok = prvi;
        for (int j = 0; j < i; j++)
            pok = pok->sljedeci;
        return pok->el;
    }
    Tip operator [] (int i) const override { 
        if (i < 0 || i >= br_el) throw std::range_error("Indeks nije validan");
        Cvor *pok = prvi;
        for (int j = 0; j < i; j++)
            pok = pok->sljedeci;
        return pok->el;
    }
    DvostrukaLista &operator = (const DvostrukaLista &l);
};

template <typename Tip>
void DvostrukaLista<Tip>::alocirajKopiju(const DvostrukaLista &l) {
    Cvor *pok1 = l.prvi, *pok2(nullptr);
    while (pok1) {
        Cvor *novi = new Cvor();
        novi->el = pok1->el;
        if (!pok2) {
            prvi = novi;
        }
        else {
            pok2->sljedeci = novi;
            pok2->sljedeci->prethodni = pok2;
        }
        pok2 = novi;
        if (pok2 == l.t) 
            t = pok2;
        pok1 = pok1->sljedeci;
    }
    posljednji = pok2;
}

template <typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator =(const DvostrukaLista<Tip> &l) {
    if (this == &l) return *this;
    while (prvi) {
        t = prvi;
        prvi = prvi->sljedeci;
        delete t;
    }
    prvi = nullptr; posljednji = nullptr; t = nullptr;
    br_el = l.br_el;
    alocirajKopiju(l);
    return *this;
}

template <typename Tip>
bool DvostrukaLista<Tip>::sljedeci() {
    if (!br_el) throw "Lista je prazna";
    if (t == posljednji) return false;
    return (t = t->sljedeci);
}

template <typename Tip>
bool DvostrukaLista<Tip>::prethodni() {
    if (!br_el) throw "Lista je prazna";
    if (prvi == t) return false;
    return (t = t->prethodni);
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi() {
    if (!br_el) throw "Lista je prazna";
    if (br_el == 1) {
        delete t; t = nullptr; prvi = nullptr; posljednji = nullptr;
    }
    else if (t == prvi) {
        prvi = prvi->sljedeci;
        delete t; t = prvi; t->prethodni = nullptr;
    }
    else if (t == posljednji) {
        posljednji = posljednji->prethodni;
        delete t; t = posljednji; t->sljedeci = nullptr;
    }
    else {
        auto pok = t;
        t = t->sljedeci;
        pok->prethodni->sljedeci = t;
        t->prethodni = pok->prethodni;
        delete pok;
    }
    br_el--;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &el) {
    Cvor *pok = new Cvor();
    pok->el = el; pok->sljedeci = nullptr; pok->prethodni = nullptr;
    if (!br_el) {
        prvi = pok; t = pok; posljednji = pok;    
    }
    else if (t == prvi) {
        prvi = pok; prvi->sljedeci = t;
        t->prethodni = prvi;
    }
    else {
        t->prethodni->sljedeci = pok;
        pok->prethodni = t->prethodni;
        pok->sljedeci = t;
        t->prethodni = pok;
    }
    br_el++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip &el) {
    Cvor *pok = new Cvor();
    pok->el = el; pok->sljedeci = nullptr; pok->prethodni = nullptr;
    if (!br_el) {
        prvi = pok; t = pok; posljednji = pok;
    }
    else if (t == posljednji) {
        t->sljedeci = pok;
        posljednji = pok;
        posljednji->prethodni = t;
    }
    else {
        pok->sljedeci = t->sljedeci;
        pok->sljedeci->prethodni = pok;
        t->sljedeci = pok;
        pok->prethodni = t;
    }
    br_el++;
}



template <typename Tip>
class DvostraniRed {
    DvostrukaLista<Tip> dl;
public:
    DvostraniRed() : dl() {}
    DvostraniRed(const DvostraniRed &dr) = default;
    ~DvostraniRed() = default;
    void brisi() {
        while (dl.brojElemenata())
            dl.obrisi();
    }
    int brojElemenata() { return dl.brojElemenata(); }
    void staviNaCelo(const Tip &el) {
        dl.pocetak();
        dl.dodajIspred(el);
    }
    Tip skiniSaCela() {
        if (!dl.brojElemenata()) throw "Prazan red";
        dl.pocetak();
        Tip celo(dl.trenutni());
        dl.obrisi();
        return celo;
    }
    Tip celo() {
        dl.pocetak();
        return dl.trenutni();
    }
    void staviNaVrh(const Tip &el) {
        dl.kraj();
        dl.dodajIza(el);
    }
    Tip skiniSaVrha() {
        if (!dl.brojElemenata()) throw "Prazan red";
        dl.kraj();
        Tip vrh(dl.trenutni());
        dl.obrisi();
        return vrh;
    }
    Tip vrh() {
        dl.kraj();
        return dl.trenutni();
    }
    DvostraniRed &operator =(const DvostraniRed &dr) = default;
};

void testStaviSkiniCelo() {
    std::cout << "Ocekivani izlaz: 5 5 4 3 2 1 0" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    DvostraniRed<int> dr;
    for (int i = 1; i <= 5; i++) dr.staviNaCelo(i);
    std::cout << dr.brojElemenata() << " ";
    while(dr.brojElemenata()) std::cout << dr.skiniSaCela() << " ";
    std::cout << dr.brojElemenata();
}

void testStaviSkiniVrh() {
    std::cout << "Ocekivani izlaz: 5 1 2 3 4 5 0" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    DvostraniRed<int> dr;
    for (int i = 1; i <= 5; i++) dr.staviNaVrh(i);
    std::cout << dr.brojElemenata() << " ";
    while(dr.brojElemenata()) std::cout << dr.skiniSaCela() << " ";
    std::cout << dr.brojElemenata();
}

void testCeloVrh() {
    std::cout << "Ocekivani izlaz: 1 2" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    DvostraniRed<int> dr;
    dr.staviNaVrh(1);
    dr.staviNaVrh(2);
    std::cout << dr.celo() << " " << dr.vrh();
}

void testBrojElemenata() {
    std::cout << "Ocekivani izlaz: 0 10" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    DvostraniRed<int> dr;
    std::cout << dr.brojElemenata() << " ";
    for (int i = 1; i <= 10; i++) dr.staviNaCelo(i);
    std::cout << dr.brojElemenata();
}



int main(){
    testBrojElemenata();
    return 0;
    
}