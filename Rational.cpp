#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <exception>
using namespace std;

int NOD(int a, int b) {
    while(a > 0 && b > 0)
        if(a > b)
            a %= b;
        else
            b %= a;
    return a + b;
}

class RationalDivisionByZero: public exception {
    const char* what() {
        return "AAAAAAAA";
    }
};


class Rational {
private:
    int p;
    int q; // is maintained to be positive

    void reduce() {
        int nod = NOD(abs(p), q);
        p = p / nod;
        q = q / nod;
    }

public:
    Rational(): p(0), q(1) {}
    //Rational(const int& a): p(a), q(1) {}
    //Rational(const Rational& r): p(r.p), q(r.q) {}
    Rational(int p_, int q_ = 1) {
        if (q_ < 0) {
            p_ *= -1;
            q_ *= -1;
        }
        if (p_ == 0) {
            q_ = 1;
        }
        p = p_;
        q = q_;
        if (p != 0) { reduce(); }
    }
    int getNumerator() const { return p; }
    int getDenominator() const { return q; }
    Rational& operator ++ () {
        *this = *this + 1;
        return *this;
    }
    Rational operator ++ (int notUsed) {
        Rational temp = *this;
        *this = *this + 1;
        return temp;
    }
    Rational& operator -- () {
        *this = *this - 1;
        return *this;
    }
    Rational operator -- (int notUsed) {
        Rational temp = *this;
        *this = *this - 1;
        return temp;
    }
    Rational& operator += (const Rational& r) {
        *this = *this + r;
        return *this;
    }
    Rational& operator -= (const Rational& r) {
        *this = *this - r;
        return *this;
    }
    Rational& operator *= (const Rational& r) {
        *this = *this * r;
        return *this;
    }
    Rational& operator /= (const Rational& r) {
        *this = *this / r;
        return *this;
    }
    friend istream& operator >> (istream&, Rational&);
    friend ostream& operator << (ostream&, const Rational&);
    friend Rational operator / (const Rational&, const Rational&);
    friend int operator > (const Rational&, const Rational&);
    friend int operator >= (const Rational&, const Rational&);
    friend int operator < (const Rational&, const Rational&);
    friend int operator <= (const Rational&, const Rational&);
    friend int operator == (const Rational&, const Rational&);
    friend int operator != (const Rational&, const Rational&);
    friend Rational operator + (const Rational&);
    friend Rational operator - (const Rational&);
    friend Rational operator + (const Rational&, const Rational&);
    friend Rational operator - (const Rational&, const Rational&);
    friend Rational operator * (const Rational&, const Rational&);
};

Rational operator / (const Rational& r1, const Rational& r2) {
    if (r2.p == 0) {
        throw RationalDivisionByZero();
    }
    Rational r3(r1.p * r2.q, r1.q * r2.p);
    return r3;
}

istream& operator >> (istream& in, Rational& r) {
    char* input = new char[100];
    in >> input;
    int length = strlen(input);
    int i = 0;
    for (int j = 0; j < length; ++j) {
        if (input[j] == '/') {
            i = j;
            break;
        }
    }
    if (i == 0) {
        r = Rational(atoi(input));
    } else {
        char* input_p = new char[50];
        for (int j = 0; j < i; ++j) {
            input_p[j] = input[j];
        }
        char* input_q = new char[50];
        for (int j = i + 1; j < length; ++j) {
            input_q[j - i - 1] = input[j];
        }
        r = Rational(atoi(input_p),atoi(input_q));
        delete[] input_p;
        delete[] input_q;
    }
    delete[] input;
    return in;
}

ostream& operator << (ostream& out, const Rational& r) {
    if (r.q == 1) {
        out << r.p;
    } else {
        out << r.p << '/' << r.q;
    }
    return out;
}

int operator > (const Rational& r1, const Rational& r2) {
    return ((double(r1.p) / double(r1.q)) > (double(r2.p) / double(r2.q)));
}

int operator >= (const Rational& r1, const Rational& r2) {
    return ((double(r1.p) / double(r1.q)) >= (double(r2.p) / double(r2.q)));
}

int operator < (const Rational& r1, const Rational& r2) {
    return ((double(r1.p) / double(r1.q)) < (double(r2.p) / double(r2.q)));
}

int operator <= (const Rational& r1, const Rational& r2) {
    return ((double(r1.p) / double(r1.q)) <= (double(r2.p) / double(r2.q)));
}

int operator == (const Rational& r1, const Rational& r2) {
    return r1.p == r2.p && r1.q == r2.q;
}

int operator != (const Rational& r1, const Rational& r2) {
    return r1.p != r2.p || r1.q != r2.q;
}

Rational operator + (const Rational& r1, const Rational& r2) {
    Rational r3(r1.p * r2.q + r2.p * r1.q, r1.q * r2.q);
    return r3;
}

Rational operator + (const Rational& r) {
    Rational new_r(r);
    return new_r;
}

Rational operator - (const Rational& r) {
    Rational new_r(-r.p, r.q);
    return new_r;
}

Rational operator * (const Rational& r1, const Rational& r2) {
    Rational r3(r1.p * r2.p, r1.q * r2.q);
    return r3;
}

Rational operator - (const Rational& r1, const Rational& r2) {
    Rational r3(r1.p * r2.q - r2.p * r1.q, r1.q * r2.q);
    return r3;
}

int main(int argc, char** argv) {
    int a;
    cin >> a;

    int p, q;
    cin >> p >> q;
    const Rational rc(p, q); // q != 0 is guaranteed by author of tests
    cout << rc.getNumerator() << ' ' << rc.getDenominator() << endl;

    Rational r1, r2;
    cin >> r1 >> r2;

    cout << r1 << endl;
    cout << r2 << endl;

    try {
        cout << 1/r1 << endl;
    } catch (const RationalDivisionByZero& ex) {
        cout << "Cannot get reciprocal of r1." << endl;
    }

    try {
        cout << rc/r2 << endl;
    } catch (const RationalDivisionByZero& ex) {
        cout << "Cannot divide by r2." << endl;
    }

    cout << (r1 < r2) << endl;
    cout << (r1 <= r2) << endl;
    cout << (r1 > r2) << endl;
    cout << (r1 >= r2) << endl;
    cout << (r1 == r2) << endl;
    cout << (r1 != r2) << endl;

    cout << (r1 < a) << endl;
    cout << (r1 <= a) << endl;
    cout << (r1 > a) << endl;
    cout << (r1 >= a) << endl;
    cout << (r1 == a) << endl;
    cout << (r1 != a) << endl;

    cout << (a < r2) << endl;
    cout << (a <= r2) << endl;
    cout << (a > r2) << endl;
    cout << (a >= r2) << endl;
    cout << (a == r2) << endl;
    cout << (a != r2) << endl;

    cout << rc + a << endl
         << a + rc << endl
         << -rc * r1 << endl
         << (+r1 - r2 * rc) * a << endl;

    cout << ++r1 << endl;
    cout << r1 << endl;
    cout << r1++ << endl;
    cout << r1 << endl;
    cout << --r1 << endl;
    cout << r1 << endl;
    cout << r1-- << endl;
    cout << r1 << endl;
    cout << ++++r1 << endl;
    cout << r1 << endl;

    cout << ((((r1 += r2) /= Rational(-5,3)) -= rc) *= a) << endl;
    cout << (r1 += r2 /= 3) << endl;
    cout << r1 << endl;
    cout << r2 << endl;
    return 0;
}