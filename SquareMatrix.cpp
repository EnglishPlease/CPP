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

class MatrixIsDegenerateError : public std::exception {
    const char* what() {
        return "AAAAAAAA";
    }
};

// non-specified functions to get "zero" and "one" of type T

template <typename T> T getZero() {
    return T(0);
}

template <typename T> T getOne() {
    return T(1);
}

//=============== Matrix class ===============//

template <typename T>
class Matrix {
protected:
    int rowsCnt;
    int colsCnt;
    T **array;
public:
    Matrix(int a, int b);
    Matrix(const Matrix<T>&);
    virtual ~Matrix();
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

//=============== SquareMatrix class ===============//

template <typename T>
class SquareMatrix : public Matrix<T> {
private:
    int size;
public:
    SquareMatrix(): Matrix<T>(), size(0) {}
    SquareMatrix(int n): Matrix<T>(n, n), size(n) {}
    SquareMatrix(const SquareMatrix& m): Matrix<T>(m), size(m.getSize()) {}
    int getSize() const;
    T getDeterminant() const;
    T getTrace() const;
    SquareMatrix getInverse() const;
    SquareMatrix invert();
    SquareMatrix getTransposed() const;
    SquareMatrix& operator = (const SquareMatrix&);
    SquareMatrix operator + (const SquareMatrix&);
    SquareMatrix operator - (const SquareMatrix&);
    template <typename X> friend SquareMatrix<X> operator * (const SquareMatrix<X>&, const SquareMatrix<X>&);
    SquareMatrix operator + (const T&);
    SquareMatrix operator - (const T&);
    SquareMatrix operator * (const T&);
    template <typename X, typename Y> friend SquareMatrix<X> operator + (const Y&, const SquareMatrix<X>&);
    template <typename X, typename Y> friend SquareMatrix<X> operator - (const Y&, const SquareMatrix<X>&);
    template <typename X> friend SquareMatrix<X> operator * (const int&, const SquareMatrix<X>&);
    template <typename X> friend SquareMatrix<X> operator * (const X&, const SquareMatrix<X>&);
    SquareMatrix& operator += (const SquareMatrix&);
    SquareMatrix& operator -= (const SquareMatrix&);
    SquareMatrix& operator *= (const SquareMatrix&);
    SquareMatrix& operator *= (const T&);
    T& operator () (int a, int b);
    T operator () (int a, int b) const;
    template <typename X> friend std::istream& operator >> (std::istream& in, SquareMatrix<X>& m);
    template <typename X> friend std::ostream& operator << (std::ostream& out, const SquareMatrix<X>& m);
};

template <typename T>
int SquareMatrix<T>::getSize() const {
    return size;
}

template <typename T>
T SquareMatrix<T>::getDeterminant() const {
    T** temp = new T*[Matrix<T>::rowsCnt];
    int swapCount = 0;
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        temp[i] = new T[Matrix<T>::rowsCnt];
        for (int j = 0; j < Matrix<T>::rowsCnt; ++j) {
            temp[i][j] = Matrix<T>::array[i][j];
        }
    }

    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        int j = i;
        for (; j < Matrix<T>::rowsCnt; ++j) {
            if (temp[j][i] != getZero<T>()) {
                break;
            }
        }
        if (j == Matrix<T>::rowsCnt) {
            return getZero<T>();
        }
        if (j > i) {
            ++swapCount;
            std::swap(temp[i], temp[j]);
        }

        for (int j = i + 1; j < Matrix<T>::rowsCnt; ++j) {
            T kf = temp[j][i] / temp[i][i];
            for (int s = i; s < Matrix<T>::rowsCnt; ++s) {
                temp[j][s] = temp[j][s] - kf * temp[i][s];
            }
        }
    }

    T det = getOne<T>();
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        det = det * temp[i][i];
    }

    return swapCount % 2 == 1 ? -det : det;
}

template <typename T>
T SquareMatrix<T>::getTrace() const {
    T sum = T(0);
    for (int i = 0; i < size; ++i) {
        sum = sum + (*this)(i, i);
    }
    return sum;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::invert() {
    T** temp = new T*[Matrix<T>::rowsCnt];

    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        temp[i] = new T[Matrix<T>::rowsCnt * 2];
    }

    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::rowsCnt * 2; ++j) {
            if (i == j - Matrix<T>::rowsCnt) {
                temp[i][j] = getOne<T>();
            }
            else {
                temp[i][j] = getZero<T>();
            }
        }
    }

    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::rowsCnt; ++j) {
            temp[i][j] = Matrix<T>::array[i][j];
        }
    }

    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        int j = i;
        for (; j < Matrix<T>::rowsCnt; ++j) {
            if (temp[j][i] != getZero<T>()) {
                break;
            }
        }
        if (j == Matrix<T>::rowsCnt) {
            throw MatrixIsDegenerateError();
        }
        if (j > i) {
            std::swap(temp[i], temp[j]);
        }

        for (int j = i + 1; j < Matrix<T>::rowsCnt; ++j) {
            T kf = temp[j][i] / temp[i][i];
            for (int s = i; s < Matrix<T>::rowsCnt * 2; ++s) {
                temp[j][s] = temp[j][s] - kf * temp[i][s];
            }
        }
    }

    for (int i = Matrix<T>::rowsCnt - 1; i >= 0; --i) {
        T res = temp[i][i];
        for (int j = i; j < Matrix<T>::rowsCnt * 2; ++j) {
            temp[i][j] = temp[i][j] / res;
        }
        for (int j = Matrix<T>::rowsCnt - 1; j >= 0; --j) {
            if (i != j) {
                res = temp[j][i];
                for (int k = 0; k < Matrix<T>::rowsCnt * 2; ++k) {
                    temp[j][k] = temp[j][k] - temp[i][k] * res;
                }
            }
        }
    }

    SquareMatrix<T> inversed(Matrix<T>::rowsCnt);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::rowsCnt; ++j) {
            inversed(i, j) = temp[i][j + Matrix<T>::rowsCnt];
        }
    }

    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::rowsCnt; ++j) {
            Matrix<T>::array[i][j] = temp[i][j + Matrix<T>::rowsCnt];
        }
    }

    return *this;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::getInverse() const {
    SquareMatrix<T> inversed = *this;
    inversed.invert();
    return inversed;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::getTransposed() const {
    SquareMatrix transposed(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            transposed(j, i) = Matrix<T>::array[i][j];
        }
    }
    return transposed;
}

template <typename T>
std::istream& operator >> (std::istream& in, SquareMatrix<T>& m) {
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            std::cin >> m.array[i][j];
        }
    }
    return in;
}

template< typename T>
std::ostream & operator << (std::ostream & out, const SquareMatrix<T>& m) {
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
T& SquareMatrix<T>::operator () (int a, int b) {
    if (a >= Matrix<T>::rowsCnt || b >= Matrix<T>::colsCnt || a < 0 || b < 0) {
        throw MatrixIndexError();
    }
    return Matrix<T>::array[a][b];
}

template <typename T>
T SquareMatrix<T>::operator () (int a, int b) const {
    if (a >= Matrix<T>::rowsCnt || b >= Matrix<T>::colsCnt || a < 0 || b < 0) {
        throw MatrixIndexError();
    }
    return Matrix<T>::array[a][b];
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator = (const SquareMatrix<T>& m) {
    if (this != &m) {
        T** new_array = new T*[m.rowsCnt];
        for (int i = 0; i < m.rowsCnt; ++i) {
            new_array[i] = new T[m.colsCnt];
        }
        for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
            delete[] Matrix<T>::array[i];
        }
        delete[] Matrix<T>::array;
        Matrix<T>::array = new_array;
        for (int i = 0; i < m.rowsCnt; ++i) {
            for (int j = 0; j < m.colsCnt; ++j) {
                Matrix<T>::array[i][j] = m.array[i][j];
            }
        }
    }
    Matrix<T>::rowsCnt = m.rowsCnt;
    Matrix<T>::colsCnt = m.colsCnt;
    return *this;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator + (const SquareMatrix<T>& m) {
    if (Matrix<T>::rowsCnt != m.rowsCnt || Matrix<T>::colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    SquareMatrix<T> new_m(*this);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) + m(i, j);
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator - (const SquareMatrix<T>& m) {
    if (Matrix<T>::rowsCnt != m.rowsCnt || Matrix<T>::colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    SquareMatrix<T> new_m(*this);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) - m(i, j);
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> operator * (const SquareMatrix<T>& m1, const SquareMatrix<T>& m2) {
    if (m2.rowsCnt != m1.colsCnt) {
        throw MatrixWrongSizeError();
    }
    SquareMatrix<T> new_m(m1.rowsCnt);
    for (int i = 0; i < m1.rowsCnt; ++i) {
        for (int j = 0; j < m2.colsCnt; ++j) {
            new_m(i, j) = MatrixSum(m1, m2, i, j, m1.colsCnt);
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator + (const T& t) {
    SquareMatrix<T> new_m(*this);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) + t;
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator - (const T& t) {
    SquareMatrix<T> new_m(*this);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) - t;
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator * (const T& t) {
    SquareMatrix<T> new_m(*this);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) * t;
        }
    }
    return new_m;
}

template <typename T, typename P>
SquareMatrix<T> operator + (const P& t, const SquareMatrix<T>& m) {
    SquareMatrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) + t;
        }
    }
    return new_m;
}

template <typename T, typename P>
SquareMatrix<T> operator - (const P& t, const SquareMatrix<T>& m) {
    SquareMatrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) - t;
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> operator * (const T& t, const SquareMatrix<T>& m) {
    SquareMatrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) * T(t);
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T> operator * (const int& t, const SquareMatrix<T>& m) {
    SquareMatrix<T> new_m(m);
    for (int i = 0; i < m.rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = new_m(i, j) * T(t);
        }
    }
    return new_m;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator += (const SquareMatrix<T>& m) {
    if (Matrix<T>::rowsCnt != m.rowsCnt || Matrix<T>::colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            (*this)(i, j) = (*this)(i, j) + m(i, j);
        }
    }
    return *this;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator -= (const SquareMatrix<T>& m) {
    if (Matrix<T>::rowsCnt != m.rowsCnt || Matrix<T>::colsCnt != m.colsCnt) {
        throw MatrixWrongSizeError();
    }
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            Matrix<T>::array[i][j] = Matrix<T>::array[i][j] - m(i, j);
        }
    }
    return *this;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator *= (const SquareMatrix<T>& m) {
    if (m.rowsCnt != Matrix<T>::colsCnt) {
        throw MatrixWrongSizeError();
    }
    SquareMatrix<T> new_m(Matrix<T>::rowsCnt);
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < m.colsCnt; ++j) {
            new_m(i, j) = MatrixSum(*this, m, i, j, Matrix<T>::colsCnt);
        }
    }
    *this = new_m;
    return *this;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator *= (const T& t) {
    for (int i = 0; i < Matrix<T>::rowsCnt; ++i) {
        for (int j = 0; j < Matrix<T>::colsCnt; ++j) {
            Matrix<T>::array[i][j] = Matrix<T>::array[i][j] * t;
        }
    }
    return *this;
}


// this works also for SquareMatrix as this does not rely on any square-specific features

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
    int m, n, p;
    Rational r;
    cin >> m >> n >> p >> r;

    Matrix<Rational> A(m, n);
    SquareMatrix<Rational> S(p);
    cin >> A >> S;

    try {
        cout << (A * S) * A.getTransposed() << endl;
    } catch (const MatrixWrongSizeError&) {
        cout << "A and S have not appropriate sizes for multiplication." << endl;
    }

    cout << (r * (S = S) * S).getSize() << endl;

    SquareMatrix<Rational> P(S);


    cout << (P * (S + S - 3 * P)).getDeterminant() << endl;

    const SquareMatrix<Rational>& rS = S;

    cout << rS.getSize() << ' ' << rS.getDeterminant() << ' ' << rS.getTrace() << endl;
    cout << (S = S) * (S + rS) << endl;
    cout << (S *= S) << endl;

    try {
        cout << rS.getInverse() << endl;
        cout << P.invert().getTransposed().getDeterminant() << endl;
        cout << P << endl;
    } catch (const MatrixIsDegenerateError&) {
        cout << "Cannot inverse matrix." << endl;
    }

    return 0;
}
