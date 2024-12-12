#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h> // ����������� ���������� ��� ������ � Windows API (����� ��� ��������� ��������� UTF-8 � PowerShel ��� CMD)

using namespace std;

// ����� ��� ������� ����������� ��������� ���������������� ������� ��������
class HeatConduction1D {
private:
    int N;                   // ���������� �����
    double L;                // ������� ��������
    double lambda;           // ����������� ����������������
    double rho;              // ��������� ���������
    double c;                // �������� ������������
    double T0;               // ��������� �����������
    double Tl;               // ����������� �� ����� ����
    double Tr;               // ����������� �� ������ ����
    double t_end;            // �����, �� �������� ����� �������
    double h;                // ��� �� ���������������� ����������
    double tau;              // ��� �� �������
    vector<double> T;   // ������ ��� �������� ����������
    vector<double> alpha, beta;  // ����������� ������������

public:
    // ����������� ������������� ���������� ������
    HeatConduction1D(int nodes, double length, double conductivity, double density, double heatCapacity,
        double initialTemp, double leftTemp, double rightTemp, double endTime)
        : N(nodes), L(length), lambda(conductivity), rho(density), c(heatCapacity),
        T0(initialTemp), Tl(leftTemp), Tr(rightTemp), t_end(endTime) {

        h = L / (N - 1);            // ������ ���� �� ������������
        tau = t_end / 100.0;        // ������ ��� �� �������
        T.resize(N, T0);            // �������������� ������ ���������� ���������� ����������
        alpha.resize(N, 0.0);       // ������ ������������� alpha ��� ������ ��������
        beta.resize(N, 0.0);        // ������ ������������� beta ��� ������ ��������
    }

    // ����� ��� ���������� ���������� ������� ��������� ����������������
    void solve() {
        double time = 0.0;
        while (time < t_end) {
            time += tau;

            // ������������� ��������� �������
            alpha[0] = 0.0;
            beta[0] = Tl;

            // ��������� ����������� ������������ alpha � beta ��� ���������� �������
            for (int i = 1; i < N - 1; ++i) {
                double ai = lambda / (h * h);
                double bi = 2.0 * lambda / (h * h) + rho * c / tau;
                double ci = lambda / (h * h);
                double fi = -rho * c * T[i] / tau;

                alpha[i] = ai / (bi - ci * alpha[i - 1]);
                beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alpha[i - 1]);
            }

            // ��������� ������ ��������� �������
            T[N - 1] = Tr;

            // �������� �������� ��� ����������� �����������
            for (int i = N - 2; i >= 0; --i) {
                T[i] = alpha[i] * T[i + 1] + beta[i];
            }
        }
    }

    // ����� ��� ������ ����������� � ��������� ����
    void saveResultsToTextFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "Temperature distribution at time t = " << t_end << " seconds:\n";
            for (int i = 0; i < N; ++i) {
                file << "x = " << i * h << ", T = " << T[i] << " C\n";
            }
            file.close();
            cout << "Results saved to file " << filename << endl;
        }
        else {
            cerr << "Error opening file for writing!" << endl;
        }
    }
    void saveResultsToTXTFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "x,temperature\n";  // ���������
            for (int i = 0; i < N; ++i) {
                file << i * h << "," << T[i] << "\n";
            }
            file.close();
            cout << "Results saved to file " << filename << endl;
        }
        else {
            cerr << "Error opening file for writing!" << endl;
        }
    }
};

// �������� �������
int main() {
    SetConsoleOutputCP(CP_UTF8);
    int N;
    double L, lambda, rho, c, T0, Tl, Tr, t_end;

    cout << "Enter the number of nodes along the spatial coordinate, N: ";
    cin >> N;
    cout << "Enter the end time, t_end (s): ";
    cin >> t_end;
    cout << "Enter the thickness of the plate, L (m): ";
    cin >> L;
    cout << "Enter the thermal conductivity, lambda (W/(m*C)): ";
    cin >> lambda;
    cout << "Enter the density, rho (kg/m^3): ";
    cin >> rho;
    cout << "Enter the specific heat capacity, c (J/(kg*C)): ";
    cin >> c;
    cout << "Enter the initial temperature, T0 (C): ";
    cin >> T0;
    cout << "Enter the temperature at the left edge, Tl (C): ";
    cin >> Tl;
    cout << "Enter the temperature at the right edge, Tr (C): ";
    cin >> Tr;

    // ������� ������ � ��������� ������
    HeatConduction1D heatConduction(N, L, lambda, rho, c, T0, Tl, Tr, t_end);
    heatConduction.solve();

    // ��������� ���������� � ��������� ����
    heatConduction.saveResultsToTextFile("oop_temp.txt");

    // ��������� ���������� � txt
    heatConduction.saveResultsToTXTFile("oop_res.txt");

    return 0;
}

