#include <iostream>
#include <utility>
#include <string>

#define KAP 1000

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() = default;
    virtual TipVrijednosti &operator [] (TipKljuca kljuc) = 0;
    virtual TipVrijednosti operator [] (TipKljuca kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kluc) = 0;
    virtual ~Mapa() {}
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti> *mapa;
    int kapacitet, brEl;
    void prosiri();
public:
    NizMapa() : mapa(new std::pair<TipKljuca, TipVrijednosti>[KAP]{}),
                kapacitet(KAP), brEl(0) {}
    TipVrijednosti &operator [] (TipKljuca kljuc) {
        for (int i = 0; i < brEl; i++)
            if (mapa[i].first == kljuc) return mapa[i].second;
        if (brEl == kapacitet) prosiri();
        mapa[brEl++] = std::make_pair(kljuc, TipVrijednosti());
        return mapa[brEl-1].second;
    }
    TipVrijednosti operator [] (TipKljuca kljuc) const {
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


void testBrojElemenata() {
    std::cout << "Ocekivani izlaz: 3" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    NizMapa<int, int> mapa;
    mapa[1] = 1;
    mapa[2] = 2;
    mapa[3] = 3;
    std::cout << mapa.brojElemenata();
}

void testOperatoraPristupa1() {
    std::cout << "Ocekivani izlaz: 5 4 3 2 1" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    NizMapa<int, int> mapa;
    for (int i = 0; i < 5; i++)
        mapa[i] = 5-i;
    for (int i = 0; i < 5; i++)
        std::cout << mapa[i] << " ";
}

void testOperatoraPristupa2() {
    std::cout << "Ocekivani izlaz: Jedan Dva Tri Cetiri Cetiri Tri Dva Jedan" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    NizMapa<int, std::string> mapa;
    mapa[1] = "Jedan";
    mapa[2] = "Dva";
    mapa[3] = "Tri";
    mapa[4] = "Cetiri";
    for (int i = 1; i < 5; i++) 
        std::cout << mapa[i] << " ";
    for (int i = 1; i < 3; i++) {
        std::string pomocni(mapa[5-i]);
        mapa[5-i] = mapa[i];
        mapa[i] = pomocni;
    }
    for (int i = 1; i < 5; i++) 
        std::cout << mapa[i] << " ";
    
}

void testObrisi2() {
    std::cout << "Ocekivani izlaz: 0 2 0 4 0 6 0 8 0 10" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    NizMapa<int, int> mapa;
    for (int i = 1; i <= 10; i++)
        mapa[i] = i;
    for (int i = 1; i <= 10; i++)
        if (i%2 == 1) mapa.obrisi(i);
    for (int i = 1; i <= 10; i++)
        std::cout << mapa[i] << " ";
}

void testObrisi1() {
    std::cout << "Ocekivani izlaz: 1" << std::endl;
    std::cout << "Dobiveni izlaz: ";
    NizMapa<int, int> mapa;
    mapa[0] = 0;
    mapa[1] = 1;
    mapa.obrisi();
    mapa[2] = 2;
    std::cout << mapa.brojElemenata();
}

int main() {
    testObrisi2();
    return 0;
}
