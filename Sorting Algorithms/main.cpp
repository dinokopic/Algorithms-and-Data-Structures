#include <iostream>
#include <fstream>

template <typename Tip>
void bubble_sort(Tip *niz, int vel) {
    for (int i = vel; i > 0; i--) {
        for (int j = 1; j < i; j++) {
            if (niz[j-1] > niz[j]) {
                Tip temp = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = temp;
            }
        }
    }
}

template <typename Tip>
void selection_sort(Tip *niz, int vel) {
    for (int i = 0; i < vel-1; i++) {
        Tip min = niz[i];
        int indeksMin = i;
        for (int j = i+1; j < vel; j++) {
            if (niz[j] < niz[indeksMin]) {
                indeksMin = j;
                min = niz[j];    
            }
        }
        niz[indeksMin] = niz[i];
        niz[i] = min;
    }
}

template <typename Tip>
void razmijeni(Tip &a, Tip &b) {
    Tip temp = a;
    a = b;
    b = temp;
}

template <typename Tip>
int particija(Tip *niz, int pocetak, int kraj) {
    Tip pivot = niz[pocetak];
    int p = pocetak + 1;
    while (p <= kraj && pivot > niz[p]) 
        p++;
    for (int i = p+1; i <= kraj; i++) {
        if (niz[i] < pivot) {
            razmijeni(niz[i], niz[p]);
            p++;
        }
    }
    razmijeni(niz[pocetak], niz[p-1]);
    return p-1;
}

template <typename Tip>
void rekurzivni_quick_sort(Tip *niz, int pocetak, int kraj) {
    if (pocetak < kraj) {
        int p = particija(niz, pocetak, kraj);
        rekurzivni_quick_sort(niz, pocetak, p-1);
        rekurzivni_quick_sort(niz, p+1, kraj);
    }
}

template <typename Tip>
void quick_sort(Tip *niz, int vel) {
    rekurzivni_quick_sort(niz, 0, vel-1);
}

template <typename Tip>
void merge(Tip *niz, int l, int p, int q, int u) {
    int i(0), j(q-l), k(l);
    Tip *pomocni = new Tip[u-l+1];
    for (int m = 0; m <= u - l; m++) 
        pomocni[m] = niz[l+m];
    while(i <= p - l && j <= u - l) {
        if (pomocni[i] < pomocni[j]) 
            niz[k++] = pomocni[i++];
        else
            niz[k++] = pomocni[j++];
    }
    while (i <= p - l) 
        niz[k++] = pomocni[i++];
    while (j <= u - l)
        niz[k++] = pomocni[j++];
    delete [] pomocni;
}

template <typename Tip>
void rekurzivni_merge_sort(Tip *niz, int l, int u) {
    if (u > l) {
        int p = (l + u - 1)/2;
        int q = p + 1;
        rekurzivni_merge_sort(niz, l, p);
        rekurzivni_merge_sort(niz, q, u);
        merge(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip *niz, int vel) {
    rekurzivni_merge_sort(niz, 0, vel-1);
}

void ucitaj(std::string filename, int*& niz, int& vel) {
    vel = 0;
    std::ifstream file(filename);
    int n;
    while(file >> n) vel++;
    file.close();
    file.open(filename);
    niz = new int[vel];
    for (int i = 0; i < vel; i++) {
        file >> n;
        niz[i] = n;
    }
    file.close();
}

void generisi(std::string filename, int vel) {
    std::ofstream file(filename);
    for (int i = 0; i < vel; i++) {
        file << rand() << " ";
    }
    file.close();
}

void program(std::string filename, int vel) {
    int *niz;
    generisi(filename, vel);
    ucitaj(filename, niz, vel);
    char izbor;
    for (int i = 0; i < vel; i++)
        std::cout << niz[i] << " ";
    std::cout << std::endl;
    std::cout << "Odaberite algoritam sortiranja: " << std::endl
    << "1. Bubble sort" << std::endl
    << "2. Selection sort" << std::endl
    << "3. Quick sort" << std::endl
    << "4. Merge sort" << std::endl;
    std::cin >> izbor;
    switch (izbor) {
        case '1' :
            bubble_sort(niz, vel);
            break;
        case '2' :
            selection_sort(niz, vel);
            break;
        case '3' :
            quick_sort(niz, vel);
            break;
        case '4' :
            merge_sort(niz, vel);
            break;
        default :
            throw "Neispravan unos";
            break;
    }
    for (int i = 0; i < vel-1; i++)
        if (niz[i] > niz[i+1]) throw "Niz nije ispravno sortiran";
    std::ofstream file("sortirano.txt");
    for (int i = 0; i < vel; i++)
        file << niz[i] << " ";
    file.close();
    std::cout << "Sortiranje uspjesno";
}



int main() {
    program("brojevi.txt", 10);
    return 0;
}
