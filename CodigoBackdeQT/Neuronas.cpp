// Codigo hecho para QTCreator

#include "Neuronas.h"
#include "./ui_mainwindow.h"
#include "Neuronas.h"
#include <QFileDialog>
#include <QTextStream>
#include <algorithm>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QPointF> // Para QPointF
#include <QLineF> // Para QLineF
#include <cmath>

Neurona::Neurona(QString id, double voltaje, double posX, double posY, int red, int green, int blue)
    : id(id), voltaje(voltaje), posX(posX), posY(posY), red(red), green(green), blue(blue)
{
    // Constructor de Neurona
}

QString Neurona::getId() const {
    return id;
}

double Neurona::getVoltaje() const {
    return voltaje;
}

double Neurona::getPosX() const {
    return posX;
}

double Neurona::getPosY() const {
    return posY;
}
int Neurona::getRed() const {
    return red;
}

int Neurona::getGreen() const {
    return green;
}

int Neurona::getBlue() const {
    return blue;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Crear una nueva escena para el QGraphicsView
    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(ui->Ingresar_Datos, &QPushButton::clicked, this, &MainWindow::ingresarDatos);
    connect(ui->pushButton_Mostrar_Por_ID, &QPushButton::clicked, this, &MainWindow::mostrarPorID);
    connect(ui->pushButton_Mostrar_Por_Voltaje, &QPushButton::clicked, this, &MainWindow::mostrarPorVoltaje);
    connect(ui->Guardar_Datos, &QPushButton::clicked, this, &MainWindow::guardarDatos);
    connect(ui->Cargar_Datos, &QPushButton::clicked, this, &MainWindow::cargarDatos);
    connect(ui->pushButton_Buscar, &QPushButton::clicked, this, &MainWindow::buscarNeurona);
    connect(ui->pushButton_OrdenarTabla_Por_ID, &QPushButton::clicked, this, &MainWindow::ordenarTablaPorID);
    connect(ui->pushButton_OrdenarTabla_Por_Voltaje, &QPushButton::clicked, this, &MainWindow::ordenarTablaPorVoltaje);
    connect(ui->pushButton_Ver_Grafica, &QPushButton::clicked, this, &MainWindow::dibujarNeuronas);
    connect(ui->Distancia_Minima, &QPushButton::clicked, this, &MainWindow::calcularDistanciasMinimas);
    connect(ui->pushButton_Nodos, &QPushButton::clicked, this, &MainWindow::grafos);
    connect(ui->button_BusquedaProfunda, &QPushButton::clicked, this, &MainWindow::busquedaProfunda);

    ui->tableWidget_Resultado->setRowCount(1);
    ui->tableWidget_Resultado->setColumnCount(7);
    ui->tableWidget_Resultado->setHorizontalHeaderLabels(QStringList() << "ID" << "Voltaje" << "Posición X" << "Posición Y" << "Red" << "Green" << "Blue");

    neuronas = QVector<Neurona>();

    // Dibujar las neuronas inicialmente
    dibujarNeuronas();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ingresarDatos()
{
    QString id = ui->Escribir_ID->text();
    double voltaje = ui->Escibir_Voltaje->text().toDouble();
    double posX = ui->Escribir_Posicion_x->text().toDouble();
    double posY = ui->Escribir_Posicion_y->text().toDouble();
    int red = ui->Escribir_Red->text().toInt();
    int green = ui->Escribir_Green->text().toInt();
    int blue = ui->Escribir_Blue->text().toInt();

    Neurona neurona(id, voltaje, posX, posY, red, green, blue);
    neuronas.append(neurona);
}

void MainWindow::mostrarPorID() {
    std::sort(neuronas.begin(), neuronas.end(), [](const Neurona& a, const Neurona& b) {
        return a.getId() < b.getId();
        });
    mostrarNeuronasEnPlainTextEdit();
}

void MainWindow::mostrarPorVoltaje() {
    std::sort(neuronas.begin(), neuronas.end(), [](const Neurona& a, const Neurona& b) {
        return a.getVoltaje() < b.getVoltaje();
        });
    mostrarNeuronasEnPlainTextEdit();
}

void MainWindow::mostrarNeuronasEnPlainTextEdit() {
    ui->plainTextEdit->clear(); // Limpiar el plainTextEdit

    // Recorrer todas las neuronas y mostrarlas en el plainTextEdit
    for (const Neurona& neurona : neuronas) {
        QString data = QString("ID: %1\nVoltaje: %2\nPosición: (%3, %4)\nRGB: (%5, %6, %7)\n")
            .arg(neurona.getId())
            .arg(neurona.getVoltaje())
            .arg(neurona.getPosX())
            .arg(neurona.getPosY())
            .arg(neurona.getRed())
            .arg(neurona.getGreen())
            .arg(neurona.getBlue());
        ui->plainTextEdit->appendPlainText(data);
    }
}

void MainWindow::guardarDatos()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar Neuronas"), "", tr("Archivo de Texto (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const Neurona& neurona : neuronas) {
                out << neurona.getId() << "," << neurona.getVoltaje() << "," << neurona.getPosX() << "," << neurona.getPosY()
                    << "," << neurona.getRed() << "," << neurona.getGreen() << "," << neurona.getBlue() << "\n";
            }
            file.close();
        }
    }
}

void MainWindow::cargarDatos()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar Neuronas"), "", tr("Archivo de Texto (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList data = line.split(",");
                if (data.size() == 7) {
                    QString id = data[0];
                    double voltaje = data[1].toDouble();
                    double posX = data[2].toDouble();
                    double posY = data[3].toDouble();
                    int red = data[4].toInt();
                    int green = data[5].toInt();
                    int blue = data[6].toInt();
                    Neurona neurona(id, voltaje, posX, posY, red, green, blue);
                    neuronas.append(neurona);
                }
            }
            file.close();
        }
    }
}

// Implementación de la búsqueda lineal
void MainWindow::buscarNeurona() {
    QString idABuscar = ui->lineEdit_Buscar->text();
    bool encontrada = false;
    int index = -1;

    // Búsqueda lineal
    for (int i = 0; i < neuronas.size(); ++i) {
        if (neuronas[i].getId() == idABuscar) {
            encontrada = true;
            index = i;
            break;
        }
    }

    if (encontrada) {
        // Mostrar resultado en la tabla
        mostrarResultadoEnTabla(index);
    }
    else {
        // Mostrar mensaje de neurona no encontrada
        QMessageBox::information(this, "Búsqueda", "La neurona con el ID especificado no fue encontrada.");
    }
}

// Implementación del método de clasificación
void MainWindow::ordenarTablaPorID() {
    // Ordenar neuronas por ID
    std::sort(neuronas.begin(), neuronas.end(), [](const Neurona& a, const Neurona& b) {
        return a.getId() < b.getId();
        });

    // Mostrar los datos ordenados en la tabla
    mostrarDatosEnTabla();
}

// Implementación del método de clasificación
void MainWindow::ordenarTablaPorVoltaje() {
    // Ordenar neuronas por ID
    std::sort(neuronas.begin(), neuronas.end(), [](const Neurona& a, const Neurona& b) {
        return a.getVoltaje() < b.getVoltaje();
        });

    // Mostrar los datos ordenados en la tabla
    mostrarDatosEnTabla();
}

// Función para mostrar el resultado de la búsqueda en la tabla
void MainWindow::mostrarResultadoEnTabla(int index) {
    // Limpiar la tabla
    ui->tableWidget_Resultado->clearContents();
    ui->tableWidget_Resultado->setRowCount(1);

    // Mostrar los datos de la neurona encontrada en la tabla
    ui->tableWidget_Resultado->setItem(0, 0, new QTableWidgetItem(neuronas[index].getId()));
    ui->tableWidget_Resultado->setItem(0, 1, new QTableWidgetItem(QString::number(neuronas[index].getVoltaje())));
    ui->tableWidget_Resultado->setItem(0, 2, new QTableWidgetItem(QString::number(neuronas[index].getPosX())));
    ui->tableWidget_Resultado->setItem(0, 3, new QTableWidgetItem(QString::number(neuronas[index].getPosY())));
    ui->tableWidget_Resultado->setItem(0, 4, new QTableWidgetItem(QString::number(neuronas[index].getRed())));
    ui->tableWidget_Resultado->setItem(0, 5, new QTableWidgetItem(QString::number(neuronas[index].getGreen())));
    ui->tableWidget_Resultado->setItem(0, 6, new QTableWidgetItem(QString::number(neuronas[index].getBlue())));
}

// Función para mostrar los datos en la tabla
void MainWindow::mostrarDatosEnTabla() {
    // Limpiar la tabla
    ui->tableWidget_Resultado->clearContents();
    ui->tableWidget_Resultado->setRowCount(neuronas.size());

    // Mostrar los datos de las neuronas en la tabla
    for (int i = 0; i < neuronas.size(); ++i) {
        ui->tableWidget_Resultado->setItem(i, 0, new QTableWidgetItem(neuronas[i].getId()));
        ui->tableWidget_Resultado->setItem(i, 1, new QTableWidgetItem(QString::number(neuronas[i].getVoltaje())));
        ui->tableWidget_Resultado->setItem(i, 2, new QTableWidgetItem(QString::number(neuronas[i].getPosX())));
        ui->tableWidget_Resultado->setItem(i, 3, new QTableWidgetItem(QString::number(neuronas[i].getPosY())));
        ui->tableWidget_Resultado->setItem(i, 4, new QTableWidgetItem(QString::number(neuronas[i].getRed())));
        ui->tableWidget_Resultado->setItem(i, 5, new QTableWidgetItem(QString::number(neuronas[i].getGreen())));
        ui->tableWidget_Resultado->setItem(i, 6, new QTableWidgetItem(QString::number(neuronas[i].getBlue())));
    }
}

// Función para dibujar las neuronas en el QGraphicsScene
void MainWindow::dibujarNeuronas() {
    // Limpiar la escena
    ui->graphicsView->scene()->clear();

    // Recorrer todas las neuronas y dibujarlas
    for (const Neurona& neurona : neuronas) {
        int size = neurona.getVoltaje(); // El tamaño del círculo dependerá del voltaje de la neurona
        int x = neurona.getPosX(); // La posición x de la neurona
        int y = neurona.getPosY(); // La posición y de la neurona
        int red = neurona.getRed(); // Componente rojo del color de la neurona
        int green = neurona.getGreen(); // Componente verde del color de la neurona
        int blue = neurona.getBlue(); // Componente azul del color de la neurona

        // Crear un nuevo objeto QGraphicsEllipseItem (círculo)
        QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x - size / 2, y - size / 2, size, size);
        // Establecer el color del círculo
        circle->setBrush(QColor(red, green, blue));
        // Agregar el círculo a la escena
        ui->graphicsView->scene()->addItem(circle);
    }
}

void MainWindow::calcularDistanciasMinimas() {
    // Vector para almacenar las conexiones entre neuronas más cercanas
    QVector<QPair<int, int>> conexiones;

    // Calcular las distancias mínimas entre cada par de neuronas
    for (int i = 0; i < neuronas.size(); ++i) {
        double distanciaMinima = std::numeric_limits<double>::max(); // Inicializar con un valor muy grande

        // Vector para almacenar las neuronas más cercanas
        QVector<int> neuronasCercanas;

        for (int j = 0; j < neuronas.size(); ++j) {
            if (i != j) { // Evitar comparar la neurona consigo misma
                double distancia = std::sqrt(std::pow(neuronas[j].getPosX() - neuronas[i].getPosX(), 2) +
                    std::pow(neuronas[j].getPosY() - neuronas[i].getPosY(), 2));

                // Si la distancia es menor que la mínima encontrada hasta ahora, actualizar la distancia mínima y reiniciar el vector de neuronas cercanas
                if (distancia < distanciaMinima) {
                    distanciaMinima = distancia;
                    neuronasCercanas.clear();
                    neuronasCercanas.append(j);
                }
                // Si la distancia es igual a la mínima encontrada hasta ahora, agregar la neurona a las neuronas cercanas
                else if (distancia == distanciaMinima) {
                    neuronasCercanas.append(j);
                }
            }
        }

        // Agregar las conexiones entre la neurona i y sus neuronas más cercanas al vector de conexiones
        for (int neuronaCercana : neuronasCercanas) {
            conexiones.append(qMakePair(i, neuronaCercana));
        }
    }

    // Dibujar líneas entre las neuronas más cercanas
    QGraphicsScene* scene = ui->graphicsView->scene();
    scene->clear(); // Limpiar la escena antes de dibujar líneas
    dibujarNeuronas();

    for (const auto& conexion : conexiones) {
        int i = conexion.first;
        int j = conexion.second;
        QGraphicsLineItem* line = new QGraphicsLineItem(neuronas[i].getPosX(), neuronas[i].getPosY(),
            neuronas[j].getPosX(), neuronas[j].getPosY());
        scene->addItem(line);
    }
}

void MainWindow::grafos() {
    // Limpiar la escena antes de dibujar el grafo
    ui->graphicsView->scene()->clear();
    dibujarNeuronas();

    // Vector para almacenar las conexiones entre neuronas más cercanas
    QVector<QPair<int, int>> conexiones;

    // Calcular las distancias mínimas entre cada par de neuronas
    for (int i = 0; i < neuronas.size(); ++i) {
        double distanciaMinima = std::numeric_limits<double>::max(); // Inicializar con un valor muy grande

        // Vector para almacenar las neuronas más cercanas
        QVector<int> neuronasCercanas;

        for (int j = 0; j < neuronas.size(); ++j) {
            if (i != j) { // Evitar comparar la neurona consigo misma
                double distancia = std::sqrt(std::pow(neuronas[j].getPosX() - neuronas[i].getPosX(), 2) +
                    std::pow(neuronas[j].getPosY() - neuronas[i].getPosY(), 2));

                // Si la distancia es menor que la mínima encontrada hasta ahora, actualizar la distancia mínima y reiniciar el vector de neuronas cercanas
                if (distancia < distanciaMinima) {
                    distanciaMinima = distancia;
                    neuronasCercanas.clear();
                    neuronasCercanas.append(j);
                }
                // Si la distancia es igual a la mínima encontrada hasta ahora, agregar la neurona a las neuronas cercanas
                else if (distancia == distanciaMinima) {
                    neuronasCercanas.append(j);
                }
            }
        }

        // Agregar las conexiones entre la neurona i y sus neuronas más cercanas al vector de conexiones
        for (int neuronaCercana : neuronasCercanas) {
            conexiones.append(qMakePair(i, neuronaCercana));
        }
    }

    // Dibujar líneas entre las neuronas más cercanas
    QGraphicsScene* scene = ui->graphicsView->scene();

    for (const auto& conexion : conexiones) {
        int i = conexion.first;
        int j = conexion.second;
        double peso = std::sqrt(std::pow(neuronas[j].getPosX() - neuronas[i].getPosX(), 2) +
            std::pow(neuronas[j].getPosY() - neuronas[i].getPosY(), 2));
        QGraphicsLineItem* line = new QGraphicsLineItem(neuronas[i].getPosX(), neuronas[i].getPosY(),
            neuronas[j].getPosX(), neuronas[j].getPosY());
        // Establecer el texto de la línea como el peso de la arista
        QString textoPeso = QString::number(peso);
        QGraphicsSimpleTextItem* texto = new QGraphicsSimpleTextItem(textoPeso);
        // Colocar el texto en el centro de la línea
        texto->setPos((neuronas[i].getPosX() + neuronas[j].getPosX()) / 2, (neuronas[i].getPosY() + neuronas[j].getPosY()) / 2);
        scene->addItem(line);
        scene->addItem(texto);
    }
}
void MainWindow::busquedaProfunda()
{
    QString id = ui->lineEdit_Busquedaprofunda->text();
    int indiceInicial = encontrarIndicePorId(id);

    if (indiceInicial == -1) {
        ui->plainTextEdit_BusquedaProfunda->setPlainText("Neurona no encontrada.");
        return;
    }

    QSet<int> visitados;
    QStringList recorrido;

    dfs(indiceInicial, visitados, recorrido);

    ui->plainTextEdit_BusquedaProfunda->setPlainText(recorrido.join(" -> "));
}
void MainWindow::dfs(int index, QSet<int>& visitados, QStringList& recorrido)
{
    QStack<int> pila;
    pila.push(index);

    while (!pila.isEmpty()) {
        int actual = pila.pop();

        if (!visitados.contains(actual)) {
            visitados.insert(actual);
            recorrido.append(neuronas[actual].getId());

            QVector<int> neuronasCercanas;
            double distanciaMinima = std::numeric_limits<double>::max();

            for (int j = 0; j < neuronas.size(); ++j) {
                if (actual != j && !visitados.contains(j)) {
                    double distancia = std::sqrt(std::pow(neuronas[j].getPosX() - neuronas[actual].getPosX(), 2) +
                        std::pow(neuronas[j].getPosY() - neuronas[actual].getPosY(), 2));
                    if (distancia < distanciaMinima) {
                        distanciaMinima = distancia;
                        neuronasCercanas.clear();
                        neuronasCercanas.append(j);
                    }
                    else if (distancia == distanciaMinima) {
                        neuronasCercanas.append(j);
                    }
                }
            }

            for (int neuronaCercana : neuronasCercanas) {
                if (!visitados.contains(neuronaCercana)) {
                    pila.push(neuronaCercana);
                }
            }
        }
    }
}

int MainWindow::encontrarIndicePorId(const QString& id)
{
    for (int i = 0; i < neuronas.size(); ++i) {
        if (neuronas[i].getId() == id) {
            return i;
        }
    }
    return -1;
}