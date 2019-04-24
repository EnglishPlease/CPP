#include <iostream>
#include <cstdlib>
#include <exception>
#include <cstring>

class MatrixAllocationError : public std::exception {
    const char* what() {
        return "AAAAAAAA";
    }
};

class MatrixWrongSizeError : public std::exception {
    const char* what() {
        return "AAAAAAAA";
    }
};

class MatrixIndexError : public std::exception {
    const char* what() {
        return "AAAAAAAA";
    }
};

// non-specified functions to get "zero" of type T

template <typename T>
T getZero() {
    return T(0);
}

//=============== Matrix class ===============//

template <typename T>
class Matrix {
private:
    int rowsCnt;
    int colsCnt;
    T **array;
public:
    Matrix(int a, int b);
    Matrix(const Matrix<T>&);
    ~Matrix();
    int getRowsNumber() const;
    int getColumnsNumber() const;
    Matrix getTransposed();
    Matrix& transpose();
    Matrix& operator = (const Matrix&);
    Matrix operator + (const Matrix&);
    Matrix operator - (const Matrix&);
    template <typename X> friend Matrix<X> operator * (const Matrix<X>&, const Matrix<X>&);
    Matrix operator + (const T&);
    Matrix operator - (const T&);
    Matrix operator * (const T&);
    template <typename X> friend Matrix<X> operator + (const X&, const Matrix<X>&);
    template <typename X> friend Matrix<X> operator - (const X&, const Matrix<X>&);
    template <typename X> friend Matrix<X> operator * (const X&, const Matrix<X>&);
    Matrix& operator += (const Matrix&);
    Matrix& operator -= (const Matrix&);
    Matrix& operator *= (const Matrix&);
    Matrix& operator *= (const T&);
    T& operator () (int a, int b);
    T operator () (int a, int b) const;
    template <typename X> friend std::istream& operator >> (std::istream& in, Matrix<X>& m);
    template <typename X> friend std::ostream& operator << (std::ostream& out, const Matrix<X>& m);
};

template <typename T>
T MatrixSum(const Matrix<T>& m1, const Matrix<T>& m2, int i, int j, int n) {
    T sum = 0;
    for (int k = 0; k < n; ++k) {
        sum = sum + m1(i, k) * m2(k, j);
    }
    return sum;
}

template <typename T>
Matrix<T>::Matrix(int a, int b) {
    rowsCnt = a;
    colsCnt = b;
    array = new T*[a];
    for (int i = 0; i < a; ++i) {
        array[i] = new T[b];
    }
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < b; ++j) {
            array[i][j] = T(0);
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& m) {
    rowsCnt = m.rowsCnt;
    colsCnt = m.colsCnt;
    T** new_array = new T*[rowsCnt];
    for (int i = 0; i < rowsCnt; ++i) {
        new_array[i] = new T[colsCnt];
    }
    array = new_array;
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            array[i][j] = m.array[i][j];
        }
    }
}

template <typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < rowsCnt; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

template <typename T>
int Matrix<T>::getRowsNumber() const {
    return rowsCnt;
}

template <typename T>
int Matrix<T>::getColumnsNumber() const {
    return colsCnt;
}

template <typename T>
Matrix<T> Matrix<T>::getTransposed() {
    Matrix transposed(colsCnt, rowsCnt);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            transposed(j, i) = array[i][j];
        }
    }
    return transposed;
}

template <typename T>
Matrix<T>& Matrix<T>::transpose() {
    *this = Matrix(this->getTransposed());
    return *this;
}

template <typename T>
std::istream& operator >> (std::istream& in, Matrix<T>& m) {
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            std::cin >> m.array[i][j];
        }
    }
    return in;
}

template< typename T>
std::ostream & operator << (std::ostream & out, const Matrix<T>& m) {
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            out << m.array[i][j];
            if (j != m.colsCnt - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }
    return out;
}

template <typename T>
T& Matrix<T>::operator () (int a, int b) {
    if (a >= rowsCnt || b >= colsCnt || a < 0 || b < 0) {
        throw MatrixIndexError();
    }
    return array[a][b];
}

template <typename T>
T Matrix<T>::operator () (int a, int b) const {
    if (a >= rowsCnt || b >= colsCnt || a < 0 || b < 0) {
        throw MatrixIndexError();
    }
    return array[a][b];
}

template <typename T>
Matrix<T>& Matrix<T>::operator = (const Matrix<T>& m) {
    if (this != &m) {
        T** new_array = new T*[m.rowsCnt];
        for (int i = 0; i < m.rowsCnt; ++i) {
            new_array[i] = new T[m.colsCnt];
        }
        for (int i = 0; i < rowsCnt; ++i) {
            delete[] array[i];
        }
        delete[] array;
        array = new_array;
        for (int i = 0; i < m.rowsCnt; ++i) {
            for (int j = 0; j < m.colsCnt; ++j) {
                array[i][j] = m.array[i][j];
            }
        }
    }
    rowsCnt = m.rowsCnt;
    colsCnt = m.colsCnt;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator + (const Matrix<T>& m) {
    if (rowsCnt != m.rowsCnt || colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    Matrix<T> new_m(*this);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) + m(i, j);
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> Matrix<T>::operator - (const Matrix<T>& m) {
    if (rowsCnt != m.rowsCnt || colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    Matrix<T> new_m(*this);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) - m(i, j);
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> operator * (const Matrix<T>& m1, const Matrix<T>& m2) {
    if (m2.rowsCnt != m1.colsCnt) {
        throw MatrixWrongSizeError();
    }
    Matrix<T> new_m(m1.rowsCnt, m2.colsCnt);
    for (int i = 0; i < m1.rowsCnt; ++i) {
        for (int j = 0; j < m2.colsCnt; ++j) {
            new_m(i, j) = MatrixSum(m1, m2, i, j, m1.colsCnt);
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> Matrix<T>::operator + (const T& t) {
    Matrix<T> new_m(*this);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) + t;
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> Matrix<T>::operator - (const T& t) {
    Matrix<T> new_m(*this);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) - t;
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> Matrix<T>::operator * (const T& t) {
    Matrix<T> new_m(*this);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) * t;
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> operator + (const T& t, const Matrix<T>& m) {
    Matrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) + t;
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> operator - (const T& t, const Matrix<T>& m) {
    Matrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) - t;
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> operator * (const T& t, const Matrix<T>& m) {
    Matrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) * t;
        }
    }
    return new_m;
}

template <typename T>
Matrix<T>& Matrix<T>::operator += (const Matrix<T>& m) {
    if (rowsCnt != m.rowsCnt || colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            (*this)(i, j) = (*this)(i, j) + m(i, j);
        }
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator -= (const Matrix<T>& m) {
    if (rowsCnt != m.rowsCnt || colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            array[i][j] = array[i][j] - m(i, j);
        }
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator *= (const Matrix<T>& m) {
    if (m.rowsCnt != colsCnt) {
        throw MatrixWrongSizeError();
    }
    Matrix<T> new_m(rowsCnt, m.colsCnt);
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = MatrixSum(*this, m, i, j, colsCnt);
        }
    }
    *this = new_m;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator *= (const T& t) {
    for (int i = 0; i < rowsCnt; ++i) {
        for (int j = 0; j < colsCnt; ++j) {
            array[i][j] = array[i][j] * t;
        }
    }
    return *this;
}

//================ class Rational ===============//

int NOD(int a, int b) {
    while (a > 0 && b > 0)
        if (a > b)
            a %= b;
        else
            b %= a;
    return a + b;
}

class RationalDivisionByZero : public std::exception {
    const char *what() {
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
    Rational() : p(0), q(1) {}

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

    Rational &operator++() {
        *this = *this + 1;
        return *this;
    }

    Rational operator++(int notUsed) {
        Rational temp = *this;
        *this = *this + 1;
        return temp;
    }

    Rational &operator--() {
        *this = *this - 1;
        return *this;
    }

    Rational operator--(int notUsed) {
        Rational temp = *this;
        *this = *this - 1;
        return temp;
    }

    Rational &operator+=(const Rational &r) {
        *this = *this + r;
        return *this;
    }

    Rational &operator-=(const Rational &r) {
        *this = *this - r;
        return *this;
    }

    Rational &operator*=(const Rational &r) {
        *this = *this * r;
        return *this;
    }

    Rational &operator/=(const Rational &r) {
        *this = *this / r;
        return *this;
    }

    friend std::istream &operator>>(std::istream &, Rational &);

    friend std::ostream &operator<<(std::ostream &, const Rational &);

    friend Rational operator/(const Rational &, const Rational &);

    friend int operator>(const Rational &, const Rational &);

    friend int operator>=(const Rational &, const Rational &);

    friend int operator<(const Rational &, const Rational &);

    friend int operator<=(const Rational &, const Rational &);

    friend int operator==(const Rational &, const Rational &);

    friend int operator!=(const Rational &, const Rational &);

    friend Rational operator+(const Rational &);

    friend Rational operator-(const Rational &);

    friend Rational operator+(const Rational &, const Rational &);

    friend Rational operator-(const Rational &, const Rational &);

    friend Rational operator*(const Rational &, const Rational &);
};

Rational operator/(const Rational &r1, const Rational &r2) {
    if (r2.p == 0) {
        throw RationalDivisionByZero();
    }
    Rational r3(r1.p * r2.q, r1.q * r2.p);
    return r3;
}

std::istream &operator>>(std::istream &in, Rational &r) {
    char *input = new char[100];
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
        char *input_p = new char[50];
        for (int j = 0; j < i; ++j) {
            input_p[j] = input[j];
        }
        char *input_q = new char[50];
        for (int j = i + 1; j < length; ++j) {
            input_q[j - i - 1] = input[j];
        }
        r = Rational(atoi(input_p), atoi(input_q));
        delete[] input_p;
        delete[] input_q;
    }
    delete[] input;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Rational &r) {
    if (r.q == 1) {
        out << r.p;
    } else {
        out << r.p << '/' << r.q;
    }
    return out;
}

int operator>(const Rational &r1, const Rational &r2) {
    return ((double(r1.p) / double(r1.q)) > (double(r2.p) / double(r2.q)));
}

int operator>=(const Rational &r1, const Rational &r2) {
    return ((double(r1.p) / double(r1.q)) >= (double(r2.p) / double(r2.q)));
}

int operator<(const Rational &r1, const Rational &r2) {
    return ((double(r1.p) / double(r1.q)) < (double(r2.p) / double(r2.q)));
}

int operator<=(const Rational &r1, const Rational &r2) {
    return ((double(r1.p) / double(r1.q)) <= (double(r2.p) / double(r2.q)));
}

int operator==(const Rational &r1, const Rational &r2) {
    return r1.p == r2.p && r1.q == r2.q;
}

int operator!=(const Rational &r1, const Rational &r2) {
    return r1.p != r2.p || r1.q != r2.q;
}

Rational operator+(const Rational &r1, const Rational &r2) {
    Rational r3(r1.p * r2.q + r2.p * r1.q, r1.q * r2.q);
    return r3;
}

Rational operator+(const Rational &r) {
    Rational new_r(r);
    return new_r;
}

Rational operator-(const Rational &r) {
    Rational new_r(-r.p, r.q);
    return new_r;
}

Rational operator*(const Rational &r1, const Rational &r2) {
    Rational r3(r1.p * r2.p, r1.q * r2.q);
    return r3;
}

Rational operator-(const Rational &r1, const Rational &r2) {
    Rational r3(r1.p * r2.q - r2.p * r1.q, r1.q * r2.q);
    return r3;
}

//=================== main() ===============//

using namespace std;

int main() {
    int m, n, p, q;
    cin >> m >> n >> p >> q;

    Matrix<int> A(m, n), B(p, q);
    cin >> A >> B;
    A = A;
    try {
        cout << A + B * 2 - m * A << endl;
        cout << (A -= B += A *= 2) << endl;
        cout << (((A -= B) += A) *= 2) << endl;
    } catch (const MatrixWrongSizeError&) {
        cout << "A and B are of different size." << endl;
    }
    B = A;

    {
        Matrix<int> AA(A);
        Matrix<int> AAA(1, 1);
        AAA = A;
        cout << AA << endl;
        cout << (AAA += Matrix<int>(m, n)) + B << endl;
    }

    Rational r;
    cin >> r;
    Matrix<Rational> C(m, n), D(p, q);
    cin >> C >> D;
    try {
        cout << C * D << endl;
        cout << (C *= D) << endl;
        cout << C << endl;
    } catch (const MatrixWrongSizeError&) {
        cout << "C and D have not appropriate sizes for multiplication." << endl;
    }
    cout << C.getTransposed() * (r * C) << endl;
    cout << C.transpose() << endl;
       try {
           (C(0, 0) *= 6) /= 3;
           cout << C(0, 0) << endl;
           cout << C(m, m) << endl;
       } catch (const MatrixIndexError&) {
           cout << "Index out of range." << endl;
       }

       {
           const Matrix<Rational>& rC = C;
           cout << rC << endl;
           cout << rC.getRowsNumber() << ' ' << rC.getColumnsNumber() << ' ' << rC(0, 0) << endl;
           cout << (C = C) * (Rational(1, 2) * rC).getTransposed() << endl;
       }
    return 0;
}
