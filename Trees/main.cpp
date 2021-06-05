#include <iostream>
#include <utility>
#include <string>
#include <ctime>

#define KAP 1000

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() = default;
    virtual TipVrijednosti &operator [] (const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti operator [] (const TipKljuca &kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kluc) = 0;
    virtual ~Mapa() {}
};


template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        Cvor(TipKljuca k, TipVrijednosti v, Cvor *r) {
            kljuc = k; vrijednost = v; 
            roditelj = r; lijevi = nullptr; desni = nullptr;
        }
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevi, *desni, *roditelj;
    };
    Cvor *mapa;
    int brEl;
    void brisanje(Cvor *pok);
    Cvor* rekurzivnoKopiraj(Cvor *pok1, Cvor *pok2, Cvor *roditelj1) {
        pok1 = new Cvor(pok2->kljuc, pok2->vrijednost, pok1);
        if (pok2->lijevi != nullptr)
            pok1->lijevi = rekurzivnoKopiraj(pok1->lijevi, pok2->lijevi, pok1);
        else 
            pok1->lijevi = nullptr;
        if (pok2->desni != nullptr)
            pok1->desni = rekurzivnoKopiraj(pok1->desni, pok2->desni, pok1);
        else 
            pok1->desni = nullptr;
        return pok1;
    }

    
public:

    BinStabloMapa() : mapa(nullptr), brEl(0) {}
    
    BinStabloMapa(const BinStabloMapa &m) : mapa(rekurzivnoKopiraj(mapa, m.mapa, nullptr)), brEl(m.brEl) {}
    
    BinStabloMapa &operator =(const BinStabloMapa &m) {
        if (this == &m) return *this;
        obrisi();
        brEl = m.brEl;
        mapa = rekurzivnoKopiraj(mapa, m.mapa, nullptr);
        return *this;
    }
    
    TipVrijednosti &operator [](const TipKljuca &kljuc) {
        Cvor *pok = mapa, *roditelj = nullptr;
        while (pok && pok->kljuc != kljuc) {
            roditelj = pok;
            if (pok->kljuc > kljuc)
                pok = pok->lijevi;
            else 
                pok = pok->desni;
        }
        if(brEl == 0) {
            mapa = new Cvor(kljuc, TipVrijednosti(), nullptr);
            pok = mapa;
            brEl++;
        }
        if (!pok) {
            if (roditelj->kljuc > kljuc) {
                roditelj->lijevi = new Cvor(kljuc, TipVrijednosti(), roditelj);
                pok = roditelj->lijevi;
            }
            else {
                roditelj->desni = new Cvor(kljuc, TipVrijednosti(), roditelj);
                pok = roditelj->desni;
            }
            brEl++;
        }
        return pok->vrijednost;
    }
    
    TipVrijednosti operator [] (const TipKljuca &kljuc) const {
        Cvor *pok = mapa;
        while (pok && pok->kljuc != kljuc) {
            if (pok->kljuc > kljuc)
                pok = pok->lijevi;
            else
                pok = pok->desni;
        }
        //if (!pok) throw "Nema elementa";
        if (!pok) return TipVrijednosti();
        return pok->vrijednost;
    }
    
    void obrisi() {
        brisanje(mapa);
        mapa = nullptr;
        brEl = 0;
    }
    
    void obrisi(const TipKljuca &kljuc) {
        Cvor *p = mapa, *q = nullptr;
        Cvor *tmp = nullptr, *pp = nullptr, *rp = nullptr;
        while (p && kljuc != p->kljuc) {
            q = p;
            if (kljuc < p->kljuc)
                p = p->lijevi;
            else 
                p = p->desni;
        }
        if (!p) return;
        if (!p->lijevi) rp = p->desni;
        else if (!p->desni) rp = p->lijevi;
        else {
            pp = p;
            rp = p->lijevi;
            tmp = rp->desni;
            while (tmp) {
                pp = rp;
                rp = tmp;
                tmp = rp->desni;
            }
            if (pp != p) {
                pp->desni = rp->lijevi;
                rp->lijevi = p->lijevi;
            }
            rp->desni = p->desni;
        }
        if (!q) mapa = rp;
        else if (p == q->lijevi)
            q->lijevi = rp;
        else q->desni = rp;
        if (rp) rp->roditelj = p->roditelj;
        delete p;
        brEl--;
    }
    
    int brojElemenata() const { return brEl; }
    
    ~BinStabloMapa() { obrisi(); }
};

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::brisanje(Cvor *pok) {
    if (!pok) return;
    if (pok->lijevi) brisanje(pok->lijevi);
    if (pok->desni) brisanje(pok->desni);
    delete pok;
}




template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        Cvor(TipKljuca k, TipVrijednosti v, Cvor *r, int b = 0) {
            kljuc = k; vrijednost = v; 
            roditelj = r; lijevi = nullptr; desni = nullptr; balans = b;
        }
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevi, *desni, *roditelj;
        int balans;
    };
    Cvor *mapa;
    int brEl;
    void brisanje(Cvor *pok);
    Cvor* rekurzivnoKopiraj(Cvor *pok1, Cvor *pok2, Cvor *roditelj1) {
        pok1 = new Cvor(pok2->kljuc, pok2->vrijednost, roditelj1);
        if (pok2->lijevi != nullptr)
            pok1->lijevi = rekurzivnoKopiraj(pok1->lijevi, pok2->lijevi, pok1);
        else 
            pok1->lijevi = nullptr;
        if (pok2->desni != nullptr)
            pok1->desni = rekurzivnoKopiraj(pok1->desni, pok2->desni, pok1);
        else 
            pok1->desni = nullptr;
        return pok1;
    }
    
    void azurirajBalans(Cvor *trenutni) {
        Cvor *r = trenutni->roditelj;
        if (trenutni->roditelj == nullptr)
            return;
        if (trenutni == r->lijevi)
            r->balans++;
        else
            r->balans--;
        if (r->balans < -1 || r->balans > 1) {
            
        }
        else azurirajBalans(r);
    }
    
public:

    AVLStabloMapa() : mapa(nullptr), brEl(0) {}
    
    AVLStabloMapa(const AVLStabloMapa &m) : mapa(rekurzivnoKopiraj(mapa, m.mapa, nullptr)), brEl(m.brEl) {}
    
    AVLStabloMapa &operator =(const AVLStabloMapa &m) {
        if (this == &m) return *this;
        obrisi();
        brEl = m.brEl;
        mapa = rekurzivnoKopiraj(mapa, m.mapa, nullptr);
        return *this;
    }
    
    TipVrijednosti &operator [](const TipKljuca &kljuc) {
        Cvor *pok = mapa, *roditelj = nullptr;
        while (pok && pok->kljuc != kljuc) {
            roditelj = pok;
            if (pok->kljuc > kljuc)
                pok = pok->lijevi;
            else 
                pok = pok->desni;
        }
        if(brEl == 0) {
            mapa = new Cvor(kljuc, TipVrijednosti(), nullptr);
            pok = mapa;
            brEl++;
        }
        if (!pok) {
            if (roditelj->kljuc > kljuc) {
                roditelj->lijevi = new Cvor(kljuc, TipVrijednosti(), roditelj);
                pok = roditelj->lijevi;
            }
            else {
                roditelj->desni = new Cvor(kljuc, TipVrijednosti(), roditelj);
                pok = roditelj->desni;
            }
            brEl++;
        }
        return pok->vrijednost;
    }
    
    TipVrijednosti operator [] (const TipKljuca &kljuc) const {
        Cvor *pok = mapa;
        while (pok && pok->kljuc != kljuc) {
            if (pok->kljuc > kljuc)
                pok = pok->lijevi;
            else
                pok = pok->desni;
        }
        //if (!pok) throw "Nema elementa";
        if (!pok) return TipVrijednosti();
        return pok->vrijednost;
    }
    
    void obrisi() {
        brisanje(mapa);
        mapa = nullptr;
        brEl = 0;
    }
    
    void obrisi(const TipKljuca &kljuc) {
        Cvor *p = mapa, *q = nullptr;
        Cvor *tmp = nullptr, *pp = nullptr, *rp = nullptr;
        while (p && kljuc != p->kljuc) {
            q = p;
            if (kljuc < p->kljuc)
                p = p->lijevi;
            else 
                p = p->desni;
        }
        if (!p) return;
        if (!p->lijevi) rp = p->desni;
        else if (!p->desni) rp = p->lijevi;
        else {
            pp = p;
            rp = p->lijevi;
            tmp = rp->desni;
            while (tmp) {
                pp = rp;
                rp = tmp;
                tmp = rp->desni;
            }
            if (pp != p) {
                pp->desni = rp->lijevi;
                rp->lijevi = p->lijevi;
            }
            rp->desni = p->desni;
        }
        if (!q) mapa = rp;
        else if (p == q->lijevi)
            q->lijevi = rp;
        else q->desni = rp;
        if (rp) rp->roditelj = p->roditelj;
        delete p;
        brEl--;
    }
    
    int brojElemenata() const { return brEl; }
    
    ~AVLStabloMapa() { obrisi(); }
};

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::brisanje(Cvor *pok) {
    if (!pok) return;
    if (pok->lijevi) brisanje(pok->lijevi);
    if (pok->desni) brisanje(pok->desni);
    delete pok;
}




int main() {
    AVLStabloMapa<int, int> mapa;
    for (int i = 1; i <= 5; i++)
        mapa[i] = i*10;
    for (int i = 1; i <= 5; i++)
        std::cout << mapa[i] << " ";
    return 0;
}


