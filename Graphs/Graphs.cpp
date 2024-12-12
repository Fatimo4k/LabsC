#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <limits>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <map>

using namespace std;

// Класс для представления вершины графа
template <class V, class E>
class Vertex;

// Класс для представления рёбер графа
template <class V, class E>
class Edge {
public:
    Edge(const E& properties, Vertex<V, E>* vertex1, Vertex<V, E>* vertex2)
        : properties_(properties), vertex1_(vertex1), vertex2_(vertex2) {}

    const Vertex<V, E>* getVertex1() const { return vertex1_; }
    const Vertex<V, E>* getVertex2() const { return vertex2_; }
    const E* getProperties() const { return &properties_; }

private:
    E properties_;
    Vertex<V, E>* vertex1_;
    Vertex<V, E>* vertex2_;
};

// Класс для представления вершины графа
template <class V, class E>
class Vertex {
public:
    ~Vertex() {
        for (Edge<V, E>* edge : edges_) {
            delete edge;
        }
    }

    void print() const {
        cout << "Вершина: " << properties_ << endl;
        for (const Edge<V, E>* edge : edges_) {
            cout << "  Ребро к " << *(edge->getVertex2()->getProperties())
                << " (стоимость: " << *(edge->getProperties()) << ")" << endl;
        }
    }

    Vertex(const V& properties) : properties_(properties) {}

    const V* getProperties() const { return &properties_; }
    const vector<Edge<V, E>*>* getEdges() const { return &edges_; }

    void addEdge(const E& properties, Vertex<V, E>* target) {
        Edge<V, E>* edge = new Edge<V, E>(properties, this, target);
        edges_.push_back(edge);
        target->edges_.push_back(edge);
    }

private:
    V properties_;
    vector<Edge<V, E>*> edges_;
};

// Посетитель для обхода графа
template <class V, class E>
class OneTimeVisitor {
public:
    bool visitVertex(const Vertex<V, E>* vertex) {
        if (find(visited_.begin(), visited_.end(), vertex) != visited_.end())
            return false;
        visited_.push_back(vertex);
        return true;
    }

    bool visitEdge(const Edge<V, E>*) { return true; }
    void leaveVertex(const Vertex<V, E>*) { visited_.pop_back(); }
    void leaveEdge(const Edge<V, E>*) {}

private:
    vector<const Vertex<V, E>*> visited_;
};

// Алгоритм обхода графа в глубину
template <class V, class E, class F>
void depthPass(const Vertex<V, E>* vertex, F* visitor) {
    if (!visitor->visitVertex(vertex))
        return;

    for (Edge<V, E>* edge : *vertex->getEdges()) {
        if (!visitor->visitEdge(edge))
            continue;

        const Vertex<V, E>* next = edge->getVertex1() == vertex ? edge->getVertex2() : edge->getVertex1();
        depthPass(next, visitor);
        visitor->leaveEdge(edge);
    }

    visitor->leaveVertex(vertex);
}

// Рекурсивная функция поиска в глубину
template <class V, class E>
bool search(const Vertex<V, E>* vertex, const V& targetName, vector<const Vertex<V, E>*>& visited, int& cost) {
    if (find(visited.begin(), visited.end(), vertex) != visited.end())
        return false;

    visited.push_back(vertex);

    if (*vertex->getProperties() == targetName) {
        cout << "Целевая вершина " << targetName << " найдена!" << endl;
        return true;
    }

    for (const Edge<V, E>* edge : *vertex->getEdges()) {
        const Vertex<V, E>* next = edge->getVertex1() == vertex ? edge->getVertex2() : edge->getVertex1();
        cout << "Переход к вершине " << *(next->getProperties())
             << " через ребро со стоимостью " << *(edge->getProperties()) << endl;

        if (search(next, targetName, visited, cost)) {
            cost += *edge->getProperties();
            cout << "Добавляем стоимость ребра: " << *next->getProperties()
                 << " (стоимость: " << *edge->getProperties()
                 << "), общая стоимость: " << cost << endl;
            return true;
        }
    }

    visited.pop_back();
    return false;
}

// Алгоритм Дейкстры для нахождения кратчайших путей
template <class V, class E>
void dijkstra(const Vertex<V, E>* start) {
    unordered_map<const Vertex<V, E>*, int> distances;
    unordered_map<const Vertex<V, E>*, const Vertex<V, E>*> previous;

    for (const Edge<V, E>* edge : *start->getEdges()) {
        const Vertex<V, E>* neighbor = edge->getVertex2();
        distances[neighbor] = numeric_limits<int>::max();
    }
    distances[start] = 0;

    auto cmp = [](pair<const Vertex<V, E>*, int>& a, pair<const Vertex<V, E>*, int>& b) {
        return a.second > b.second;
    };
    priority_queue<pair<const Vertex<V, E>*, int>, vector<pair<const Vertex<V, E>*, int>>, decltype(cmp)> pq(cmp);
    pq.push({ start, 0 });

    while (!pq.empty()) {
        const Vertex<V, E>* current = pq.top().first;
        pq.pop();

        for (const Edge<V, E>* edge : *current->getEdges()) {
            const Vertex<V, E>* neighbor = edge->getVertex2();
            int newDist = distances[current] + *edge->getProperties();

            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = current;
                pq.push({ neighbor, newDist });
            }
        }
    }

    cout << "Результаты алгоритма Дейкстры:" << endl;
    for (const auto& pair : distances) {
        cout << "Расстояние до вершины " << *(pair.first->getProperties())
             << " равно " << pair.second << endl;
    }
}

// Функция для чтения вершин из файла
vector<string> readVertices(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return {};
    }

    vector<string> lines;
    string line;

    while (getline(file, line) && lines.size() < 3) {
        lines.push_back(line);
    }

    return lines;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Создание графа
    Vertex<string, int> A1("A1");
    Vertex<string, int> B1("B1");
    Vertex<string, int> C1("C1");
    Vertex<string, int> B2("B2");
    Vertex<string, int> E1("E1");
    Vertex<string, int> F1("F1");
    Vertex<string, int> B3("B3");

    // Добавление рёбер с указанием стоимости
    A1.addEdge(10, &B1);
    B1.addEdge(20, &C1);
    A1.addEdge(15, &B2);
    C1.addEdge(30, &E1);
    E1.addEdge(25, &F1);
    B2.addEdge(40, &F1);
    A1.addEdge(10, &B3);
    B3.addEdge(12, &C1);

    // Вызов метода print для каждой вершины
    cout << "Информация о графе:" << endl;
    A1.print();
    B1.print();
    C1.print();
    B2.print();
    E1.print();
    F1.print();
    B3.print();

    // Пример поиска пути
    vector<const Vertex<string, int>*> visited; // Вектор для хранения посещенных вершин
    // Переменная для хранения стоимости пути
    int cost = 0;

    // Создание отображения вершин
    map<string, Vertex<string, int>*> vertexMap;
    vertexMap["A1"] = &A1;
    vertexMap["B1"] = &B1;
    vertexMap["C1"] = &C1;
    vertexMap["B2"] = &B2;
    vertexMap["E1"] = &E1;
    vertexMap["F1"] = &F1;
    vertexMap["B3"] = &B3;

    // Чтение вершин из файла
    string filename = "vershina.txt"; // Имя файла
    vector<string> vertices = readVertices(filename); // Читаем вершины из файла

    // Проверяем, прочитали ли мы три строки
    if (vertices.size() < 3) {
        cout << "Файл не найден или содержит менее трёх строк." << endl;
        return 1; // Завершаем программу, если не удалось прочитать три строки
    }

    string firstVertex = vertices[0];
    string secondVertex = vertices[1];
    string dijkstraVertex = vertices[2];

    // Поиск первой вершины в отображении
    auto it = vertexMap.find(firstVertex);
    if (it != vertexMap.end()) {
        // Если вершина найдена, запускаем алгоритмы
        OneTimeVisitor<string, int> visitor;
        cout << "Обход графа с использованием OneTimeVisitor:" << endl;
        depthPass(it->second, &visitor);

        // Запускаем алгоритм Дейкстры от второй вершины
        auto itDijkstra = vertexMap.find(secondVertex);
        if (itDijkstra != vertexMap.end()) {
            // Передаем строку как string для поиска
            if (search(it->second, secondVertex, visited, cost)) {
                cout << "Путь найден, стоимость: " << cost << endl;
            }
            else {
                cout << "Путь не найден." << endl;
            }

            cout << "Запускаем алгоритм Дейкстры от вершины: " << dijkstraVertex << endl;
            auto itDijkstraStart = vertexMap.find(dijkstraVertex);
            if (itDijkstraStart != vertexMap.end()) {
                dijkstra(itDijkstraStart->second); // Запускаем алгоритм Дейкстры от найденной вершины
            }
            else {
                cout << "Вершина для алгоритма Дейкстры не найдена в графе." << endl;
            }
        }
        else {
            cout << "Вершина для поиска пути не найдена в графе." << endl;
        }
    }
    else {
        cout << "Вершина не найдена в графе." << endl;
    }

    return 0;
}

