#include "database.h"

Database::~Database() {

    this->m_dataBase.close();
}

void Database::write(const QString &data, const QByteArray &array, const float comparison_result) {

    QSqlQuery query;

    query.prepare(

        "INSERT INTO ScreenTable (Time, Image, Result)"
        " VALUES "
        "(:TABLE_TIME, :TABLE_IMAGE, :TABLE_RESULT );"
        );

    query.bindValue(":TABLE_TIME"     ,data);
    query.bindValue(":TABLE_IMAGE"  ,array);
    query.bindValue(":TABLE_RESULT" ,comparison_result);

    if(!query.exec()) qDebug() << query.lastError().text();
}

void Database::read(QueryResult &queryResult) {

    QSqlQuery query;

    query.prepare( "SELECT * FROM " TABLE );

     if(!query.exec()) qDebug() << query.lastError().text();

    while(query.next()) {

        queryResult.push(

                    query.value(1).toString(),
                    query.value(2).toByteArray(),
                    query.value(3).toInt());
    }
}

bool Database::connectionDataBase() {

    m_dataBase = QSqlDatabase::addDatabase("QSQLITE");
    m_dataBase.setDatabaseName("YawarePNG.db");
    if(m_dataBase.open()) return true;
    else return false;
}

bool Database::createDataBase() {

    m_dataBase = QSqlDatabase::addDatabase("QSQLITE");
    m_dataBase.setDatabaseName("YawarePNG.db");

    if(!m_dataBase.open()) return false;
    else {
        if(createTable()) return true;
        else return false;
    }
}

bool Database::createTable() {

     QSqlQuery query;

    if(!query.exec( "CREATE TABLE " TABLE " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    TABLE_TIME        " VARCHAR(255)  NOT NULL,"
                    TABLE_IMAGE      " BLOB                  NOT NULL,"
                    TABLE_RESULT     " INTEGER           NOT NULL"
                    " )"
                    )){
        qDebug() << query.lastError().text();
        return false;
    }
    else return true;
}
