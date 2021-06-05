#include <iostream>

bool daLiJePotencija(int broj, int baza) {
    if (baza > broj) return false;
    else if (baza == broj) return true;
    return daLiJePotencija(broj/baza, baza);
}

int main() {
    std::cout << daLiJePotencija(12,2);
}