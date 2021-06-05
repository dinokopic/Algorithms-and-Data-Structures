#include <iostream>
#include <stdexcept>
#include <string>

template <typename Tip>
class Stek {
    struct Cvor {
        Tip el;
        Cvor *sljedeci;
    };
    Cvor *top;
    int brEl;
public:
    Stek() : top(nullptr), brEl(0) {}
    ~Stek() { brisi(); }
    Stek(const Stek &s);
    Stek &operator =(const Stek &s);
    void brisi();
    void stavi(const Tip &el);
    Tip skini();
    Tip &vrh() const { 
        if (!brEl) throw std::domain_error("Prazan stek");
        return top->el; 
    }
    int brojElemenata() const { return brEl; }
};

template <typename Tip>
void Stek<Tip>::brisi() {
    Cvor *pok = top;
    while (top) {
        top = top->sljedeci;
        delete pok;
        pok = top;
    }
    brEl = 0;
}

template <typename Tip>
Tip Stek<Tip>::skini() {
    if (!brEl) throw std::domain_error("Prazan stek");
    Tip element = top->el;
    auto pok = top;
    top = top->sljedeci;
    delete pok;
    brEl--;
    return element;
}

template <typename Tip>
void Stek<Tip>::stavi(const Tip &el) {
    Cvor *novi = new Cvor;
    novi->el = el;
    novi->sljedeci = top;
    top = novi;
    brEl++;
}

template <typename Tip>
Stek<Tip>::Stek(const Stek<Tip> &s) : top(nullptr), brEl(0) {
    Cvor *pok1 = s.top, *pok2(nullptr);
    while (pok1) {
        if (!top) {
            top = new Cvor;
            pok2 = top;
            top->sljedeci = nullptr;
        }
        else {
            top->sljedeci = new Cvor;
            top = top->sljedeci;
        }
        top->el = pok1->el;
        pok1 = pok1->sljedeci;
    }
    top->sljedeci = nullptr;
    top = pok2;
    brEl = s.brEl;
}

template <typename Tip>
Stek<Tip> &Stek<Tip>::operator =(const Stek<Tip> &s) {
    if (this == &s) return *this;
    brisi();
    Cvor *pok1 = s.top, *pok2(nullptr);
    while (pok1) {
        if (!top) {
            top = new Cvor;
            pok2 = top;
            top->sljedeci = nullptr;
        }
        else {
            top->sljedeci = new Cvor;
            top = top->sljedeci;
        }
        top->el = pok1->el;
        pok1 = pok1->sljedeci;
    }
    top->sljedeci = nullptr;
    top = pok2;
    brEl = s.brEl;
    return *this;
}


bool provjeri_zagrade (std::string tekst) {
    Stek<char> stek;
    for (int i = 0; i < tekst.size(); i++) {
        if (tekst[i] != '{' && tekst[i] != '[' && tekst[i] != '(' && tekst[i] != ')' && tekst[i] != ']' && tekst[i] != '}')
            continue;
        if (tekst[i] == '{' || tekst[i] == '[' || tekst[i] == '(') stek.stavi(tekst[i]);
        else if (stek.brojElemenata() != 0 && ((tekst[i] == '}' && stek.vrh() == '{') || (tekst[i] == ']' && stek.vrh() == '[') || (tekst[i] == ')' && stek.vrh() == '(')))
            stek.skini();
        else return false;
    }
    return (stek.brojElemenata() == 0);
}

int main() {
    std::cout << provjeri_zagrade("{[()]}");
    return 0;
}
