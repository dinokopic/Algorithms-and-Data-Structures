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
    friend class Iterator<Tip>;
};


template <typename Tip>
void DvostrukaLista<Tip>::alocirajKopiju(const DvostrukaLista &l) {
    Cvor *pok1 = l.prvi, *pok2(nullptr);
    while (pok1) {
        Cvor *novi = new Cvor();
        novi->el = pok1->el;
        if (!pok2) {
            prvi = novi;
            //prvi->prethodni = nullptr;
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
class Iterator {
    const DvostrukaLista<Tip> *lista;
    typename DvostrukaLista<Tip>::Cvor *t;
    void praznaTest() const {
        if (!lista->br_el) throw "Lista prazna";
    }
public:
    Iterator(const DvostrukaLista<Tip> &lista) : lista(&lista), t(lista.t) {}
    Tip trenutni() const {
        praznaTest();
        return t->el;
    }
    bool prethodni() {
        praznaTest();
        if (t == lista->prvi) return false;
        return (t = t->prethodni);
    }
    bool sljedeci() {
        praznaTest();
        if (t == lista->posljednji) return false;
        return (t = t->sljedeci);
    }
    void pocetak() {
        praznaTest();
        t = lista->prvi;
    }
    void kraj() {
        praznaTest();
        t = lista->posljednji;
    }
};

template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n) {
    Iterator<Tip> it = dynamic_cast<const DvostrukaLista<Tip>&>(n);
    it.pocetak();
    Tip maksimum(it.trenutni());
    while (it.sljedeci()) 
        if (it.trenutni() > maksimum) maksimum = it.trenutni(); 
    return maksimum;
}



void testDodajIspred() {
    std::cout << "Ocekivani izlaz: 2 3 4 5 1" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    for (int i = 1; i <= 5; i++) l.dodajIspred(i);
    for (int i = 0; i < 5; i++) std::cout << l[i] << " ";
    std::cout << std::endl;
}

void testDodajIza() {
    std::cout << "Ocekivani izlaz: 1 5 4 3 2" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    for (int i = 1; i <= 5; i++) l.dodajIza(i);
    for (int i = 0; i < 5; i++) std::cout << l[i] << " ";
    std::cout << std::endl;
}

void testTrenutni1() {
    std::cout << "Ocekivani izlaz: 3" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    l.dodajIza(3);
    l.dodajIspred(2);
    l.dodajIza(1);
    std::cout << l.trenutni() << std::endl;
}

void testTrenutni2() {
    std::cout << "Ocekivani izlaz: 10" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    l.dodajIza(3);
    l.dodajIspred(2);
    l.dodajIza(1);
    l.trenutni() = 10;
    std::cout << l.trenutni() << std::endl;
}

void testSljedeci1() {
    std::cout << "Ocekivani izlaz: 0" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    l.dodajIza(1);
    std::cout << l.sljedeci() << std::endl;
}

void testSljedeci2() {
    std::cout << "Ocekivani izlaz: 1 5" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    l.dodajIza(4);
    l.dodajIza(5);
    std::cout << l.sljedeci() << " " << l.trenutni() << std::endl;
}

void testPrethodni1() {
    std::cout << "Ocekivani izlaz: 0" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    l.dodajIza(1);
    std::cout << l.prethodni() << std::endl;
}

void testPrethodni2() {
    std::cout << "Ocekivani izlaz: 1 5" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    l.dodajIspred(4);
    l.dodajIspred(5);
    std::cout << l.prethodni() << " " << l.trenutni() << std::endl;
}

void testPocetakIKraj() {
     std::cout << "Ocekivani izlaz: 6 2" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    for (int i = 1; i <= 5; i++) l.dodajIza(i);
    for (int i = 6; i <= 10; i++) l.dodajIspred(i);
    l.pocetak();
    std::cout << l.trenutni() << " "; 
    l.kraj();
    std::cout << l.trenutni() << std::endl;
}

void testBrojElemenata() {
    std::cout << "Ocekivani izlaz: 5" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    for (int i = 1; i <= 5; i++) l.dodajIspred(i);
    std::cout << l.brojElemenata() << std::endl;
}

void testObrisi() {
    std::cout << "Ocekivani izlaz: 4" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    for (int i = 1; i <= 5; i++) l.dodajIspred(i);
    l.obrisi();
    std::cout << l.brojElemenata() << std::endl;
}

void testOperatorPristupa() {
    std::cout << "Ocekivani izlaz: 4" << std::endl 
    << "Dobiveni izlaz: ";
    DvostrukaLista<int> l;
    for (int i = 1; i <= 5; i++) l.dodajIspred(i);
    std::cout << l[2] << std::endl;
}

int main(){
    return 0;
    
}