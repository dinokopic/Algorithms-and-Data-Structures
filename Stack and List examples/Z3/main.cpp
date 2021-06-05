#include <iostream>
#include <algorithm>
#include <stdexcept>

#define VELICINA_LISTE 1000

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
};

template <typename Tip>
class NizLista : public Lista<Tip> {
    Tip **niz;
    int kapacitet, br_el, t;
    void testIndeksa(int indeks) const;
    void povecajKapacitet();
    void obrisiNiz(Tip **pok, int n) {
        for (int i = 0; i < n; i++) delete pok[i];
        delete[] pok;
    }
public:
    NizLista() : niz(new Tip*[VELICINA_LISTE]{}), kapacitet(VELICINA_LISTE), 
        br_el(0), t(0) {};
    ~NizLista() {
        obrisiNiz(niz, br_el);
    }
    NizLista(const NizLista &nl) : niz(new Tip*[nl.kapacitet]{}), kapacitet(nl.kapacitet), 
        br_el(nl.br_el), t(nl.t) {
        for (int i = 0; i < br_el; i++)
            niz[i] = new Tip(*nl.niz[i]);
    }
    int brojElemenata() const override { return br_el; }
    Tip& trenutni() override { 
        if (br_el == 0) throw ("NizLista je prazna");
        return *niz[t];
    }
    Tip trenutni() const override {
        if (br_el == 0) throw ("NizLista je prazna");
        return *niz[t];
    }
    bool sljedeci() override {
        if (br_el == 0) throw "NizLista je prazna";
        if (t == br_el-1) return false;
        return ++t;
    }
    bool prethodni() override {
        if (br_el == 0) throw "NizLista je prazna";
        if (t == 0) return false;
        return --t;
    }
    void dodajIza(const Tip &el) override;
    void dodajIspred(const Tip &el) override;
    void obrisi() override;
    void pocetak() override {
        if (br_el == 0) throw "NizLista je prazna";
        t = 0;
    }
    void kraj() override {
        if (br_el == 0) throw "NizLista je prazna";
        t = br_el-1;
    }
    NizLista &operator = (const NizLista &nl);
    Tip &operator[](int i) override {
        testIndeksa(i); return *niz[i];
    }
    Tip operator[](int i) const override {
        testIndeksa(i); return *niz[i];
    }
};

template <typename Tip>
class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip el;
        Cvor *sljedeci;
    };
    Cvor *prvi, *posljednji, *t;
    int br_el;
    void alocirajKopiju(const JednostrukaLista &l);
public:
    JednostrukaLista() : prvi(nullptr), posljednji(nullptr), t(nullptr), br_el(0) {}
    JednostrukaLista(const JednostrukaLista &l) : br_el(l.br_el) {
        alocirajKopiju(l);    
    }
    ~JednostrukaLista() {
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
    JednostrukaLista &operator = (const JednostrukaLista &l);
};


template <typename Tip>
void NizLista<Tip>::dodajIza(const Tip &el) {
    if (br_el == kapacitet) povecajKapacitet();
    Tip *novi = new Tip(el);
    for (int i = br_el; i > t+1; i--) 
        niz[i] = niz[i-1];
    if (br_el == 0) niz[t] = novi;
    else niz[t+1] = novi;
    br_el++;
}

template <typename Tip>
void NizLista<Tip>::dodajIspred(const Tip &el) {
    if (br_el == kapacitet) povecajKapacitet();
    Tip *novi = new Tip(el);
    for (int i = br_el; i > t; i--)
        niz[i] = niz[i-1];
    niz[t] = novi;
    br_el++;
    if (br_el != 1) t++;
}

template <typename Tip>
void NizLista<Tip>::povecajKapacitet() {
    kapacitet *= 2;
    Tip **novi = new Tip*[kapacitet]{};
    for (int i = 0; i < br_el; i++) 
        novi[i] = niz[i];
    delete[] niz;
    niz = novi;
}

template <typename Tip>
void NizLista<Tip>::obrisi() {
    if (br_el == 0) throw "NizLista je prazna";
    delete niz[t];
    for (int i = t; i < br_el-1; i++)
        niz[i] = niz[i+1];
    niz[br_el-1] = nullptr;
    br_el--;
    if (t == br_el && br_el > 0) t--;
}

template <typename Tip>
void NizLista<Tip>::testIndeksa(int indeks) const {
    if (indeks < 0 || indeks >= br_el) 
        throw std::range_error("Indeks nije validan");
}

template <typename Tip>
NizLista<Tip> &NizLista<Tip>::operator = (const NizLista<Tip> &nl) {
    if (this == &nl) return *this;
    auto pomocni = new Tip*[nl.kapacitet]{};
    for (int i = 0; i < br_el; i++)
        pomocni[i] = new Tip(*nl.niz[i]);
    for (int i = 0; i < br_el; i++) 
        delete niz[i];
    delete[] niz;
    kapacitet = nl.kapacitet;
    br_el = nl.br_el;
    t = nl.t;
    niz = pomocni;
    return *this;
}

template <typename Tip>
void JednostrukaLista<Tip>::alocirajKopiju(const JednostrukaLista &l) {
    Cvor *pok1 = l.prvi, *pok2(nullptr);
    while (pok1) {
        Cvor *novi = new Cvor();
        novi->el = pok1->el;
        if (!pok2) 
            prvi = novi;
        else 
            pok2->sljedeci = novi;
        pok2 = novi;
        if (pok2 == l.t) 
            t = pok2;
        pok1 = pok1->sljedeci;
    }
    posljednji = pok2;
}

template <typename Tip>
JednostrukaLista<Tip> &JednostrukaLista<Tip>::operator =(const JednostrukaLista<Tip> &l) {
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
bool JednostrukaLista<Tip>::sljedeci() {
    if (!br_el) throw "Lista je prazna";
    if (t == posljednji) return false;
    return (t = t->sljedeci);
}

template <typename Tip>
bool JednostrukaLista<Tip>::prethodni() {
    if (!br_el) throw "Lista je prazna";
    if (prvi == t) return false;
    auto pok = prvi;
    while (pok->sljedeci != t) pok = pok->sljedeci;
    return (t = pok);
}

template <typename Tip>
void JednostrukaLista<Tip>::obrisi() {
    if (!br_el) throw "Lista je prazna";
    if (br_el == 1) {
        delete t; t = nullptr; prvi = nullptr; posljednji = nullptr;
    }
    else if (t == prvi) {
        prvi = prvi->sljedeci;
        delete t; t = prvi;
    }
    else if (t == posljednji) {
        prethodni();
        delete posljednji; posljednji = t; posljednji->sljedeci = nullptr;
    }
    else {
        auto pok = t;
        prethodni();
        t->sljedeci = pok->sljedeci;
        t = t->sljedeci;
        delete pok;
    }
    br_el--;
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip &el) {
    Cvor *pok = new Cvor;
    pok->el = el; pok->sljedeci = nullptr;
    if (br_el && prethodni()) {
        pok->sljedeci = t->sljedeci;
        t->sljedeci = pok;
        t = pok->sljedeci;
    }
    else {
        pok->sljedeci = prvi;
        prvi = pok;
        if (!br_el) {
            t = pok; posljednji = pok;
        }
    }
    br_el++;
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip &el) {
    Cvor *pok = new Cvor;
    pok->el = el; pok->sljedeci = nullptr;
    if (!br_el) {
        prvi = pok; t = pok; posljednji = pok;
    }
    else if (t == posljednji) {
        t->sljedeci = pok;
        posljednji = pok;
    }
    else {
        pok->sljedeci = t->sljedeci;
        t->sljedeci = pok;
    }
    br_el++;
}




bool palindrom(const Lista<char> &rijec) {
    int n = rijec.brojElemenata();
    for (int i = 0; i < n/2; i++) {
        if(rijec[i] != rijec[n-i-1]) return false;
    }
    return true;
}




int main() {
    Lista<char> *lista = new NizLista<char>;
    lista->dodajIza('R');
    lista->dodajIza('A'); lista->kraj();
    lista->dodajIza('T'); lista->kraj();
    lista->dodajIza('L'); lista->kraj();
    lista->dodajIza('R'); lista->kraj();
    std::cout << palindrom(*lista);
    delete lista;
    return 0;
}








