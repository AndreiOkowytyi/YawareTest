#include "queryresults.h"

void QueryResult::push(const QString data, const QByteArray Image, const short result) {

    this->m_q_byteArray.push(Image);
    this->m_q_string.push(data);
    this->m_q_short.push(result);
};

QByteArray QueryResult::getByteArray() {

  QByteArray temp_QByteArray = this->m_q_byteArray.front();
  this->m_q_byteArray.pop();
  return temp_QByteArray;
}

QString QueryResult::getDatay() {

  QString temp = this->m_q_string.front();
  this->m_q_string.pop();
  return temp;
}

short QueryResult::getResult() {

  short temp = this->m_q_short.front();
  this->m_q_short.pop();
  return temp;
}
