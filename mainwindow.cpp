#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionValidator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setInputBytes();

    ui->bytesValue->setInputMask("HH:HH:HH:HH:HH:HH:HH:HH");
    ui->bytesValue->setText("00:00:00:00:00:00:00:00");


    connect(ui->isTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &MainWindow::onComboType);

    // инициализация таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::runProcess);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}


void MainWindow::onComboType(int index){
    if (index == 0) {
        ui->timerSpin->setEnabled(false);
    } else {
        ui->timerSpin->setEnabled(true);
    }
}



void MainWindow::on_strtBtn_clicked()
{
    QString sourceDir = ui->sourcePath->text();
    QString resultDir = ui->resultPath->text();
    QString byteString = ui->bytesValue->text();

    // проверка, что директории выбраны
    if (sourceDir.isEmpty() || resultDir.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбраны директории.");
        return;
    }

    // получаем bytesValue
    QByteArray byteArray = QByteArray::fromHex(byteString.toUtf8()); // Преобразуем введенные данные в массив байтов
    if (byteArray.size() != 8) {
        QMessageBox::warning(this, "Ошибка", "Необходимо ввести 8 байт.");
        return;
    }

    // блокируем кнопки после проверки ввода
    ui->strtBtn->setEnabled(false);
    ui->isDeleteBox->setEnabled(false);
    ui->isTypeBox->setEnabled(false);
    ui->maskEdit->setEnabled(false);
    ui->sourcePath->setEnabled(false);
    ui->resultPath->setEnabled(false);
    ui->bytesValue->setEnabled(false);
    ui->isMatchBox->setEnabled(false);
    ui->sourceDir->setEnabled(false);
    ui->resultDir->setEnabled(false);

    // разблокируем timerSpin только если выбрана работа по таймеру
    if (ui->isTypeBox->currentText() == "Работа по таймеру") {
        ui->timerSpin->setEnabled(true);
        int delay = ui->timerSpin->value() * 1000;
        timer->start(delay);

        QMessageBox::information(this, "Таймер", "Процесс запустится через " + QString::number(ui->timerSpin->value()) + " секунд.");
    } else {
        // если выбран "Разовый запуск", сразу выполняем процесс
        runProcess();
    }
}

void MainWindow::runProcess()
{
    timer->stop();
    QString sourceDir = ui->sourcePath->text();
    QString resultDir = ui->resultPath->text();
    QString mask = ui->maskEdit->text();

    if (sourceDir.isEmpty() || resultDir.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбраны директории.");
        return;
    }

    // преобразуем введенные данные в массив байтов
    QString byteString = ui->bytesValue->text();
    QByteArray byteArray = QByteArray::fromHex(byteString.toUtf8());

    if (byteArray.size() != 8) {
        QMessageBox::warning(this, "Ошибка", "Необходимо ввести 8 байт!");
        return;
    }

    QDir sourceDirectory(sourceDir);
    // получаем список файлов в директории
    QStringList files = sourceDirectory.entryList(QDir::Files);

    for (const QString &fileName : files) {
        // проверка на соответствие маске
        if (fileName.contains(mask)) {
            QString newFileName = resultDir + "/" + QFileInfo(fileName).baseName() + "_mod";

            // проверка на существование файла, добавление индекса
            int counter = 0;
            QString finalFileName = newFileName;
            while (QFile::exists(finalFileName + "." + QFileInfo(fileName).suffix())) {
                counter++;
                finalFileName = newFileName + "_" + QString::number(counter); // Формируем имя с индексом
            }
            finalFileName += "." + QFileInfo(fileName).suffix(); // Добавляем расширение к имени

            // Проверка на доступность файла
            QFile inputFile(sourceDir + "/" + fileName);
            if (!inputFile.open(QIODevice::ReadOnly)) {
                QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл: " + fileName);
                continue;
            }

            QByteArray fileData = inputFile.readAll();
            inputFile.close();

            // xor
            for (int i = 0; i < fileData.size(); i++) {
                fileData[i] ^= byteArray[i % 8]; // Выполняем операцию XOR
            }

            // запись в новый файл
            QFile outputFile(finalFileName);
            if (outputFile.open(QIODevice::WriteOnly)) {
                outputFile.write(fileData);
                outputFile.close();
                QMessageBox::information(this, "Успех", "Файл модифицирован: " + finalFileName);
            } else {
                QMessageBox::warning(this, "Ошибка", "Не удалось создать файл: " + finalFileName);
            }

            // удаление исходного файла, если выбрано "да" в isDeleteBox
            if (ui->isDeleteBox->currentText() == "Да") {
                if (QFile::remove(sourceDir + "/" + fileName)) {
                    qDebug() << "Исходный файл удалён: " + fileName;
                } else {
                    QMessageBox::warning(this, "Ошибка", "Не удалось удалить файл: " + fileName);
                }
            }
        }
    }

    ui->strtBtn->setEnabled(true);
    ui->isDeleteBox->setEnabled(true);
    ui->isTypeBox->setEnabled(true);
    ui->maskEdit->setEnabled(true);
    ui->sourcePath->setEnabled(true);
    ui->resultPath->setEnabled(true);
    ui->bytesValue->setEnabled(true);
    ui->timerSpin->setEnabled(true);
    ui->isMatchBox->setEnabled(true);
    ui->sourceDir->setEnabled(true);
    ui->resultDir->setEnabled(true);



    QMessageBox::information(this, "Успех", "Обработка завершена.");
}





void MainWindow::on_sourceDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите директорию"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->sourcePath->setText(dir);
    }
}

void MainWindow::on_resultDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите директорию"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->resultPath->setText(dir);
    }
}

void MainWindow::setInputBytes(){

    //максимум 16 символов для 8 байт
    QRegularExpression hexRegex("^[0-9A-Fa-f]{0,16}$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(hexRegex, this);

    ui->bytesValue->setValidator(validator);

}



