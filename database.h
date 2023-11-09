#pragma once

#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "queryresults.h"

#define TABLE                "ScreenTable" // Название таблицы.
#define TABLE_TIME     "Time"            // Вторая колонка.
#define TABLE_IMAGE   "Image"         // Третья колонка.
#define TABLE_RESULT "Result"        // Четвертая колонка.

class Database { // Работа с базой данных.
 public:
    ~Database();

    bool connectionDataBase();                                                                                                          // Подключение к базе.
    bool createDataBase();                                                                                                                   // Создание базы данных.
    void write(const QString &data, const QByteArray &array, const float comparison_result); // Запись в базу данных.
    void read(QueryResult &queryResult);                                                                                         // Чтение из базы данных.

 private:
    bool createTable();  // Запрос на создание таблицы.

    QSqlDatabase m_dataBase;
};

#endif // DATABASE_H
