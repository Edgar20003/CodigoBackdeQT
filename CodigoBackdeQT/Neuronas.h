#ifndef NEURONAS_H
#define NEURONAS_H

// Codigo hecho para QTCreator...

#include "ui_mainwindow.h" para la interfaz
#include <QMainWindow>
#include <QString>
#include <QVector> // Incluir esta línea para usar QVector
#include <QGraphicsScene> // Para QGraphicsScene
#include <QGraphicsView> // Para QGraphicsView
#include <QStack>
#include <QSet>

class Neurona
{
public:
    Neurona(QString id, double voltaje, double posX, double posY, int red, int green, int blue);
    QString getId() const;
    double getVoltaje() const;
    double getPosX() const;
    double getPosY() const;
    int getRed() const;
    int getGreen() const;
    int getBlue() const;
    QPointF getPosicion() const { return QPointF(posX, posY); }

private:
    QString id;
    double voltaje;
    double posX;
    double posY;
    int red;
    int green;
    int blue;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void ingresarDatos();
    void mostrarPorID();
    void mostrarPorVoltaje();
    void mostrarNeuronasEnPlainTextEdit();
    void guardarDatos();
    void cargarDatos();
    void buscarNeurona();
    void ordenarTablaPorID();
    void ordenarTablaPorVoltaje();
    void mostrarResultadoEnTabla(int index);
    void mostrarDatosEnTabla();
    void dibujarNeuronas();
    void calcularDistanciasMinimas();
    void grafos();
    void busquedaProfunda();

private:
    Ui::MainWindow* ui;
    QVector<Neurona> neuronas; // Declarar el contenedor de neuronas

    void dfs(int index, QSet<int>& visitados, QStringList& recorrido);
    int encontrarIndicePorId(const QString& id);
};

#endif // NEURONAS_H