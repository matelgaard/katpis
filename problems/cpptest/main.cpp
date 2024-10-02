using namespace std;
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    int ticks;

    cin >> ticks;

    double revolutions = ticks / 4.0;

    cout.precision(2);
    cout << fixed << revolutions << endl;
}
