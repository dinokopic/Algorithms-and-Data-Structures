#include <iostream>

double pow(double a, unsigned int n) {
    if (n == 1) return a;
    int neparan(n%2);
    double z = pow(a, (n-neparan)/2);
    if (neparan) return z*z*a;
    return z*z;
}

int main() {
    std::cout << pow(2,11);
    return 0;
}