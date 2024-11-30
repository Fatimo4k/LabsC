#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T = double>
class MatrixDense {
    unsigned _m; // Количество строк
    unsigned _n; // Количество столбцов
    T* _data;    // Динамический массив для хранения элементов матрицы

public:
    // Конструктор
    MatrixDense(unsigned m, unsigned n) : _m(m), _n(n) {
        _data = new T[_m * _n]();
    }

    // Деструктор
    ~MatrixDense() {
        delete[] _data;
    }

    // Установка элемента
    void setElement(unsigned i, unsigned j, T value) {
        if (i >= _m || j >= _n) {
            throw out_of_range("Index out of range");
        }
        _data[j + i * _n] = value;
    }

    // Получение элемента
    T getElement(unsigned i, unsigned j) const {
        if (i >= _m || j >= _n) {
            throw out_of_range("Index out of range");
        }
        return _data[j + i * _n];
    }

    // Сложение матриц
    MatrixDense<T> add(const MatrixDense<T>& other) const {
        if (_m != other._m || _n != other._n) {
            throw invalid_argument("Matrix dimensions must match for addition");
        }
        MatrixDense<T> result(_m, _n);
        for (unsigned i = 0; i < _m; i++) {
            for (unsigned j = 0; j < _n; j++) {
                result.setElement(i, j, this->getElement(i, j) + other.getElement(i, j));
            }
        }
        return result;
    }

    // Вычитание матриц
    MatrixDense<T> subtract(const MatrixDense<T>& other) const {
        if (_m != other._m || _n != other._n) {
            throw invalid_argument("Matrix dimensions must match for subtraction");
        }
        MatrixDense<T> result(_m, _n);
        for (unsigned i = 0; i < _m; i++) {
            for (unsigned j = 0; j < _n; j++) {
                result.setElement(i, j, this->getElement(i, j) - other.getElement(i, j));
            }
        }
        return result;
    }

    // Умножение матриц
    MatrixDense<T> multiply(const MatrixDense<T>& other) const {
        if (_n != other._m) {
            throw invalid_argument("Matrix dimensions must match for multiplication");
        }
        MatrixDense<T> result(_m, other._n);
        for (unsigned i = 0; i < _m; i++) {
            for (unsigned j = 0; j < other._n; j++) {
                T sum = 0;
                for (unsigned k = 0; k < _n; k++) {
                    sum += this->getElement(i, k) * other.getElement(k, j);
                }
                result.setElement(i, j, sum);
            }
        }
        return result;
    }


    // Вывод матрицы
    void Print() const {
        for (unsigned i = 0; i < _m; i++) {
            for (unsigned j = 0; j < _n; j++) {
                cout << getElement(i, j) << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    MatrixDense<double> mat1(2, 2);
    MatrixDense<double> mat2(2, 2);

    mat1.setElement(0, 0, 1);
    mat1.setElement(0, 1, 2);
    mat1.setElement(1, 0, 3);
    mat1.setElement(1, 1, 4);

    mat2.setElement(0, 0, 5);
    mat2.setElement(0, 1, 6);
    mat2.setElement(1, 0, 7);
    mat2.setElement(1, 1, 8);

    cout << "Matrix 1:" << endl;
    mat1.Print();

    cout << "Matrix 2:" << endl;
    mat2.Print();

    MatrixDense<double> sum = mat1.add(mat2);
    cout << "Sum:" << endl;
    sum.Print();

    MatrixDense<double> difference = mat1.subtract(mat2);
    cout << "Difference:" << endl;
    difference.Print();

    MatrixDense<double> product = mat1.multiply(mat2);
    cout << "Product:" << endl;
    product.Print();

    return 0;
}