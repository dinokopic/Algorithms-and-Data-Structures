#include <iostream>

int fib2_0(int n, int pretposljednji = 0, int posljednji = 1) {
    if (n <= 1) return n;
    if (n == 2) return pretposljednji + posljednji;
    return fib2_0(n-1, posljednji, pretposljednji + posljednji);
}

int nzd(int x, int y) {
    if (y == 0) return x;
    return nzd(y, x % y);
}

int main() {
    std::cout << fib2_0(20);
    return 0;
}
