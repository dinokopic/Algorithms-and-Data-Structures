#include <iostream>

template <typename Tip>
int binarnaPretraga(Tip *niz, int n, Tip element) {
    int pocetak = 0, kraj = n-1, sredina;
    while (kraj >= pocetak) {
        sredina = (pocetak + kraj)/2;
        if (niz[sredina] == element)
            return sredina;
        else if (niz[sredina] < element) 
            pocetak = sredina + 1;
        else 
            kraj = sredina - 1;
    }
    return -1;
}

int main() {
    int niz[] = {1,3,5,6,7,9,10,17,22};
    std::cout << binarnaPretraga(niz, 9, 2);
    return 0;
}