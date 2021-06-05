#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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


void radixSort(std::vector<int> &a) {
    Red<int> red[10];
    bool gotovo = false;
    int i = 10;
    while (!gotovo) {
        for (int x : a) 
            red[(x%i)*10/i].stavi(x);
        gotovo = true;
        int k = 0;
        for (int j = 0; j < 10; j++) 
            while (red[j].brojElemenata()) {
                if (red[j].celo() >= i)
                    gotovo = false;
                a[k++] = red[j].skini();
            }
        i*= 10;
    }

}

//////////////////////////////////////////////////////////////

int roditelj(int i) {
    return (i-1)/2;
}

int lijevoDijete(int i) {
    return 2*i + 1;
}

int desnoDijete(int i) {
    return 2*i + 2;
}

bool jeLiList(const vector<int> &v, int i, int velicina) {
    return i >= velicina/2 && i < velicina; 
}

void razmijeni(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void popraviDolje(vector<int> &v, int i, int velicina) {
    while (!jeLiList(v,i,velicina)) {
        int veci(lijevoDijete(i));
        int dd(desnoDijete(i));
        if (dd < velicina && v[dd] > v[veci])
            veci = dd;
        if (v[i] > v[veci])
            return;
        razmijeni(v[i], v[veci]);
        i = veci;
    }
}

void popraviGore(vector<int> &v, int i) {
    while (i > 0 && v[i] > v[roditelj(i)]) {
        razmijeni(v[i], v[roditelj(i)]);
        i = roditelj(i);
    }
}

void stvoriGomilu(vector<int> &v) {
    for (int i = v.size()/2; i >= 0; i--)
        popraviDolje(v,i,v.size());
}

void umetniUGomilu(vector<int> &v, int umetnuti, int &velicina) {
    if (velicina == v.size()) {
        v.push_back(0);
    }
    v[velicina++] = umetnuti;
    popraviGore(v,velicina-1);
}

int izbaciPrvi(vector<int> &v, int &velicina) {
    if (velicina == 0)
        throw "Gomila je prazna!";
    velicina--;
    razmijeni(v[0], v[velicina]);
    if (velicina != 0)
        popraviDolje(v, 0, velicina);
    return v[velicina];
}

void gomilaSort(vector<int> &v) {
    stvoriGomilu(v);
    int velicina = v.size();
    for (int i = v.size()-1; i > 0; i--) {
        razmijeni(v[0], v[i]);
        velicina--;
        popraviDolje(v,0,velicina);
    }
}



int main() {
    vector<int> a;
    a.push_back(546);
    a.push_back(1);
    a.push_back(31);
    a.push_back(1001);
    a.push_back(104351);
    a.push_back(7);
    a.push_back(6);
    a.push_back(60045);
    gomilaSort(a);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<" ";
    }
    return 0;
}
