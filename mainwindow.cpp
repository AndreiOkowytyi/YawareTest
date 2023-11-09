#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), p_ui(new Ui::MainWindow) {

    this->p_ui->setupUi(this);

    this->p_Database      = new Database;
    this->p_queryResult  = new QueryResult;
    this->p_timer             = new QTimer(this);

    QObject::connect(this->p_timer, &QTimer::timeout, this, &MainWindow::screen);
    QObject::connect(this->p_ui->requestDataPushButton,  &QPushButton::clicked , this,&MainWindow::creatTable);
    QObject::connect(this->p_ui->startPushButton, &QPushButton::clicked, this, [this]() {

        this->p_timer->start(std::chrono::minutes(1));
    });

    QObject::connect(this->p_ui->stopPushButton, &QPushButton::clicked, this, [this]() {

        this->p_timer->stop();
    });

    QObject::connect(this->p_ui->tableWidget, &QTableWidget::clicked, this, [this] (const QModelIndex& model) {

        this->m_v_ByteImage[model.row()];
        QPixmap image;
        image.loadFromData(this->m_v_ByteImage.at(model.row()), "PNG");
        this->p_ui->label->setPixmap(image.scaled(300, 200));
    });

   connectionDatabase();
   creatingTableHeader();
}

MainWindow::~MainWindow() {

    delete this->p_ui;
    delete this->p_Database;
    delete this->p_queryResult;
}

void MainWindow::creatingTableHeader() {

    this->p_ui->tableWidget->setColumnCount(2);
    this->p_ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Дата створення запису" << "Процентне співідношення збігання зображення в %");
    this->p_ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::imageInput() {

    QPixmap image;
    image.loadFromData(this->m_v_ByteImage.at(0), "PNG");
    this->p_ui->label->setPixmap(image.scaled(300, 200));
}

void MainWindow::connectionDatabase() {

    if(QFile::exists("YawarePNG.db")) { // Проверка на наличие файла базы данных.

        // Подключение к базе данных.

        if(this->p_Database->connectionDataBase()) p_Database->read(*p_queryResult);
        else this->p_ui->labelError->setText("Помилка. Невірна робота з базой данных.");

        // Если файл небыл найден, создается новая база данных.

    } else  {

        if(!this->p_Database->createDataBase()) this->p_ui->labelError->setText("Помилка. Невірна робота з базой данных.");
        this->p_ui->requestDataPushButton->setEnabled(false);
    }
}

void MainWindow::creatTable() {

     while(!this->p_queryResult->m_q_short.empty()) {

        this->p_ui->tableWidget->insertRow(0);
        this->m_v_ByteImage.emplace(this->m_v_ByteImage.begin(), this->p_queryResult->getByteArray());
        this->p_ui->tableWidget->setItem(0 ,0,  new QTableWidgetItem((p_queryResult->getDatay())));
        this->p_ui->tableWidget->setItem(0 ,1,  new QTableWidgetItem(QString::number(p_queryResult->getResult())));
    }
      imageInput();
}

void MainWindow::screen() {

    QScreen *screen = QApplication::primaryScreen();
    QPixmap inPixmap = screen->grabWindow(0);
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );

    m_v_ByteImage.insert(m_v_ByteImage.begin() ,inByteArray);

    std::thread thread([this](){

       auto p_Comparison = std::make_unique<ComparisonImage>();
       QObject::connect(p_Comparison.get(), SIGNAL(resultComparison(short)), this, SLOT(writeDatabase(short)));
       p_Comparison->comparison(this->m_v_ByteImage);
    } );

    thread.join();
}

void MainWindow::writeDatabase(short result) {

    this->p_ui->tableWidget->insertRow(0);
    this->p_ui->tableWidget->setItem(0 ,0, new QTableWidgetItem(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")));
    this->p_ui->tableWidget->setItem(0 ,1, new QTableWidgetItem(QString::number(result)));

    imageInput();

    this->p_Database->write(this->p_ui->tableWidget->item(0,0)->text(), m_v_ByteImage.front(), result);
}


void ComparisonImage::comparison(const std::vector<QByteArray> &byteImage) {

    if (byteImage.size() > 1) {

    int comparison_count = 0;

    QByteArray temp_comparison_1 = byteImage[0];
    QByteArray temp_comparison_2 = byteImage[1];

    auto min = std::min(temp_comparison_1.size(), temp_comparison_2.size());

    for(int x = 0; x  < min; ++x)
        if(temp_comparison_1.at(x) == temp_comparison_2.at(x))  comparison_count++;;

    emit resultComparison(static_cast<short>((static_cast<float>(comparison_count) / min) * 100));

    } else emit resultComparison(100);
}
