#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <QScreen>
#include <QPixmap>
#include <vector>
#include <thread>
#include <QLabel>
#include <QTableWidget>

#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ComparisonImage;

class MainWindow : public QMainWindow {
    Q_OBJECT
 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private:
    void creatingTableHeader(); // Заполнения заголвка таблицы.
    void imageInput();                 // Вывод изображения.
    void connectionDatabase();  // Подключение к базе данных.

    Ui::MainWindow *p_ui          = nullptr;
    Database     *p_Database       = nullptr;
    QTimer        *p_timer             = nullptr;
    QueryResult *p_queryResult = nullptr;

    std::vector<QByteArray> m_v_ByteImage; // Хранение изображений.

 public slots:
    void screen();                                   // Реализация выполнения скриншота главного изображения.
    void writeDatabase(float result);  // Запись в базу данных.
    void creatTable();                            //  Создание таблицы исходя из данных что были получены из базу данных.
};


class ComparisonImage : public QObject { // Класс для  сравнения массивов QByteArray.
    Q_OBJECT
 public:
    void comparison(const std::vector<QByteArray> &byteImage); // Реализация сравнения массива QByteArray.

 signals:
     void resultComparison(float);
};
#endif // MAINWINDOW_H
