#pragma once

#ifndef QUERYRESULTS_H
#define QUERYRESULTS_H

#include <QByteArray>
#include <QString>
#include <queue>

// Реализация  простого контейнера для хранения результатов запроса к базе данных.

struct QueryResult {
 public:
   void push(const QString data, const QByteArray Image, const short result); // Запись данных в контейнер.

   QByteArray getByteArray();
   QString getDatay();
   short getResult();

   std::queue<QByteArray> m_q_byteArray;
   std::queue<short>            m_q_short;
   std::queue<QString>        m_q_string;
};

#endif // QUERYRESULTS_H
