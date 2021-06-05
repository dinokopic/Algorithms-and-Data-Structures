#include <iostream>
#include <utility>
#include <string>

#define KAP 1000

unsigned int hash(int ulaz, unsigned int max) {
    unsigned int suma=5381;
// 5381 je pocetna vrijednost koja poboljsava distribuciju
    while (ulaz != 0) {
        suma = suma*33 + ulaz%10;
        ulaz /= 10;
    }
    return suma % max;
}

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
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti> *mapa;
    int *popunjeno;
    int kapacitet, brEl;
    unsigned int (*hashFunkcija)(TipKljuca, unsigned int);
    void prosiri();
    
    int hashUmetni(TipKljuca kljuc) {
        for (int i = 0; i < kapacitet; i++) {
            int indeks = (hashFunkcija(kljuc, kapacitet)+i)%kapacitet;
            if (popunjeno[indeks] != 1) {
                popunjeno[indeks] = 1;
                mapa[indeks] = std::make_pair(kljuc, TipVrijednosti());
                return indeks;
            }
        }
        return -1;
    }
    int hashTrazi(TipKljuca kljuc) const {
        for (int i = 0; i < kapacitet; i++) {
            int indeks = (hashFunkcija(kljuc, kapacitet)+i)%kapacitet;
            if (popunjeno[indeks] == 0) break;
            if (popunjeno[indeks] == 1 && mapa[indeks].first == kljuc)
                return indeks;
        }
        return -1;
    }
    
public:
    HashMapa() : mapa(new std::pair<TipKljuca, TipVrijednosti>[KAP]{}), popunjeno(new int[KAP]{}),
                kapacitet(KAP), brEl(0), hashFunkcija(nullptr) {}
                
    TipVrijednosti &operator [] (const TipKljuca &kljuc) {
        if (hashFunkcija == nullptr) throw "Hash funkcija nije definisana";
        int indeks = hashTrazi(kljuc);
        if (indeks != -1 && popunjeno[indeks] == 1)
            return mapa[indeks].second;
        if (brEl == kapacitet) prosiri();
        indeks = hashUmetni(kljuc);
        brEl++;
        return mapa[indeks].second;
    }
    TipVrijednosti operator [] (const TipKljuca &kljuc) const {
        int indeks = hashTrazi(kljuc);
        if (indeks != -1)
            return mapa[indeks].second;
        return TipVrijednosti();
    }
    int brojElemenata() const { return brEl; }
    void obrisi() { 
        brEl = 0; 
        delete [] popunjeno;
        popunjeno = new int[kapacitet]{};
    }
    void obrisi(const TipKljuca &kljuc);
    ~HashMapa() { delete [] mapa; delete [] popunjeno; }
    HashMapa(const HashMapa &nmapa);
    HashMapa &operator = (const HashMapa &m);
    void definisiHashFunkciju(unsigned int (*funkcija)(TipKljuca, unsigned int)) {
        hashFunkcija = funkcija;
    }

};

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::prosiri() {
    kapacitet *= 2;
    std::pair<TipKljuca, TipVrijednosti> *pomocni = new std::pair<TipKljuca, TipVrijednosti>[kapacitet]{};
    int *pomocniObrisani = new int[kapacitet]{};
    for (int i = 0; i < brEl; i++) {
        pomocni[i] = mapa[i];
        pomocniObrisani[i] = popunjeno[i];     
    }
    delete [] mapa;
    delete [] popunjeno;
    mapa = pomocni;
    popunjeno = pomocniObrisani;
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
    int indeks = hashTrazi(kljuc);
    if (popunjeno[indeks] != 1) throw "Nema kljuca";
    popunjeno[indeks] = -1;
    brEl--;
} 

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(const HashMapa<TipKljuca, TipVrijednosti> &m) :
                mapa(new std::pair<TipKljuca, TipVrijednosti>[m.kapacitet]{}), popunjeno(new int[m.kapacitet]{}), 
                kapacitet(m.kapacitet), brEl(m.brEl), hashFunkcija(m.hashFunkcija) {
    for (int i = 0; i < kapacitet; i++) {
        popunjeno[i] = m.popunjeno[i];
        if (popunjeno[i] == 1)
            mapa[i] = m.mapa[i];
    }
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>& HashMapa<TipKljuca, TipVrijednosti>::operator = (const HashMapa<TipKljuca, TipVrijednosti> &m) {
    if (&m == this) return *this;
    delete [] mapa;
    delete [] popunjeno;
    kapacitet = m.kapacitet;
    brEl = m.brEl;
    mapa = new std::pair<TipKljuca, TipVrijednosti>[kapacitet]{};
    popunjeno = new int[kapacitet]{};
    hashFunkcija = m.hashFunkcija;
    for (int i = 0; i < kapacitet; i++) {
        popunjeno[i] = m.popunjeno[i];
        if (popunjeno[i] == 1)
            mapa[i] = m.mapa[i];
    }
    return *this;
}


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
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti> *mapa;
    int kapacitet, brEl;
    void prosiri();
public:
    NizMapa() : mapa(new std::pair<TipKljuca, TipVrijednosti>[KAP]{}),
                kapacitet(KAP), brEl(0) {}
    TipVrijednosti &operator [] (const TipKljuca &kljuc) {
        for (int i = 0; i < brEl; i++)
            if (mapa[i].first == kljuc) return mapa[i].second;
        if (brEl == kapacitet) prosiri();
        mapa[brEl++] = std::make_pair(kljuc, TipVrijednosti());
        return mapa[brEl-1].second;
    }
    TipVrijednosti operator [] (const TipKljuca &kljuc) const {
        for (int i = 0; i < brEl; i++) 
            if (mapa[i].first == kljuc) return mapa[i].second;
        return TipVrijednosti();
    }
    int brojElemenata() const { return brEl; }
    void obrisi() { brEl = 0; }
    void obrisi(const TipKljuca &kljuc);
    ~NizMapa() { delete [] mapa; }
    NizMapa(const NizMapa &nmapa);
    NizMapa &operator = (const NizMapa &m);
};

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::prosiri() {
    kapacitet *= 2;
    std::pair<TipKljuca, TipVrijednosti> *pomocni = new std::pair<TipKljuca, TipVrijednosti>[kapacitet]{};
    for (int i = 0; i < brEl; i++) pomocni[i] = mapa[i];
    delete [] mapa;
    mapa = pomocni;
}

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
    for (int i = 0; i < brEl; i++) {
        if (mapa[i].first == kljuc) {
            for (int j = i+1; j < brEl; j++)
                mapa[j-1] = mapa[j];
            brEl--;
            return;
        }
    }
    throw "Nema kljuca";
} 

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &m) :
                mapa(new std::pair<TipKljuca, TipVrijednosti>[m.kapacitet]{}), brEl(m.brEl) {
    for (int i = 0; i < brEl; i++) 
        mapa[i] = m.mapa[i];
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>& NizMapa<TipKljuca, TipVrijednosti>::operator = (const NizMapa<TipKljuca, TipVrijednosti> &m) {
    if (&m == this) return *this;
    delete [] mapa;
    kapacitet = m.kapacitet;
    brEl = m.brEl;
    mapa = new std::pair<TipKljuca, TipVrijednosti>[m.kapacitet]{};
    for (int i = 0; i < brEl; i++) 
        mapa[i] = m.mapa[i];
    return *this;
}

void testPerformansi() {
    HashMapa<int, int> hmapa;
    hmapa.definisiHashFunkciju(hash);
    BinStabloMapa<int, int> bmapa;
    NizMapa<int, int> nmapa;
    int niz[10], vel = 0;
    std::cout << "Hash mapa --- BS mapa --- Niz mapa" << std::endl;
    for (int i = 0; i < 500; i++) {
        int x = rand();
        int y = rand();
        clock_t vrijemeHMapa1 = clock();
        hmapa[x] = y;
        clock_t vrijemeHMapa2 = clock();
        clock_t vrijemeBMapa1 = clock();
        bmapa[x] = y;
        clock_t vrijemeBMapa2 = clock();
        clock_t vrijemeNizMapa1 = clock();
        nmapa[x] = y;
        clock_t vrijemeNizMapa2 = clock();
        if (i % 50 == 0) {
            niz[vel++] = x;
            std::cout << vrijemeHMapa2 - vrijemeHMapa1 << "ms --- "
            << vrijemeBMapa2 - vrijemeBMapa1 << "ms --- " 
            << vrijemeNizMapa2 - vrijemeNizMapa1 << "ms" << std::endl;   
        }
    }
    //mozemo primjetiti da, pri dodavanju 1000 elemenata, vrijeme umetanja u mapu
    //implementiranu preko binarnog stabla je skoro pa konstantno, dok vrijeme
    //umetanja u mapu implementiranu preko niza postepeno raste
    
    //sto se tice hash mape, ukoliko ne dodje do kolizije, vrijeme umetanja pri ovakvoj implementaciji
    //je malo bolje nego za binarno stablo, dok u slucaju kolizije
    //performanse su znatno slabije (ali i dalje bolje nego za niz mapu)
    
    std::cout << std::endl << "Hash mapa --- BS mapa --- Niz mapa" << std::endl;
    
    for (int i = 0; i < vel; i++) {
        std::cout << std::endl;
        clock_t vrijemeHMapa1 = clock();
        std::cout << hmapa[niz[i]];
        clock_t vrijemeHMapa2 = clock();
        std::cout << " --- ";
        clock_t vrijemeBMapa1 = clock();
        std::cout << bmapa[niz[i]];
        clock_t vrijemeBMapa2 = clock();
        std::cout << " --- ";
        clock_t vrijemeNizMapa1 = clock();
        std::cout << nmapa[niz[i]];
        clock_t vrijemeNizMapa2 = clock();
        std::cout << std::endl;
        std::cout << vrijemeHMapa2 - vrijemeHMapa1 << "ms --- "
        << vrijemeBMapa2 - vrijemeBMapa1 << "ms --- " 
        << vrijemeNizMapa2 - vrijemeNizMapa1 << "ms" << std::endl;
    }
    //u prosjeku je mapa implementirana preko binarnog stabla brza (skoro pa duplo), od
    //mape implementirane preko niza (osim u rijetkim slucajevima)
    
    //performasne hash mape su donekle bolje od preformansi binarnog stabla,
    //ali pri desavanju kolizije performanse su dosta oslabljene
}


int main() {
    testPerformansi();
    return 0;
}
