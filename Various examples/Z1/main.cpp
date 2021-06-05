#include <iostream>
#include <algorithm>
#include <stdexcept>

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
class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip el;
        Cvor *sljedeci;
    };
    Cvor *prvi, *posljednji, *t;
    int br_el;
    void alocirajKopiju(const JednostrukaLista &l);
    void rekurzivniIspisUnazad(Cvor *pok);
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
    void ispisiUnazad();
};

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

template <typename Tip>
void JednostrukaLista<Tip>::rekurzivniIspisUnazad(Cvor *pok) {
    if (!pok) return;
    else rekurzivniIspisUnazad(pok->sljedeci);
    std::cout << pok->el << " ";
}

template <typename Tip>
void JednostrukaLista<Tip>::ispisiUnazad() {
    rekurzivniIspisUnazad(prvi);
}



int main(){
    JednostrukaLista<int> l;
    for (int i = 1; i <= 10; i++) l.dodajIza(i);
    l.ispisiUnazad();
    return 0;
    
}