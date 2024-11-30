#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
vector<int> readDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Unable to open file: " + filename);
    }

    int count;
    file >> count;

    vector<int> data(count);
    for (int i = 0; i < count; ++i) {
        file >> data[i];
    }

    file.close();
    return data;
}

vector<int> Sum(const vector<int>& vec1, const vector<int>& vec2) {
    size_t size = min(vec1.size(), vec2.size());
    vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
    return result;
}

vector<int> Dif(const vector<int>& vec1, const vector<int>& vec2) {
    size_t size = min(vec1.size(), vec2.size());
    vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = vec1[i] - vec2[i];
    }
    return result;
}

vector<int> Mul(const vector<int>& vec1, const vector<int>& vec2) {
    size_t size = min(vec1.size(), vec2.size());
    vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = vec1[i] * vec2[i];
    }
    return result;
}

void printVector(const vector<int>& vec) {
    for (int value : vec) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    try {
        vector<int> data1 = readDataFromFile("file1.txt");
        vector<int> data2 = readDataFromFile("file2.txt");

        vector<int> sum = Sum(data1, data2);
        cout << "Summ of Vectors: ";
        printVector(sum);


        vector<int> dif = Dif(data1, data2);
        cout << "Dif of Vectors: ";
        printVector(dif);

        vector<int> mul = Mul(data1, data2);
        cout << "Mul of Vectors: ";
        printVector(mul);




    }
    catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }

    return 0;
}