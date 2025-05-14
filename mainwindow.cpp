#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(new QSettings(getConfigPath(), QSettings::IniFormat))
{
    ui->setupUi(this);

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_currentIndexChanged);

    loadLastFolder();
}

MainWindow::~MainWindow()
{
    saveLastFolder();
    delete ui;
}

QString MainWindow::getConfigPath()
{
    QString appDir = QCoreApplication::applicationDirPath();
    return appDir + "/config.ini";
}

void MainWindow::loadLastFolder()
{
    QString lastFolder = settings->value("LastFolder", QDir::homePath()).toString();

    if (!lastFolder.isEmpty() && QDir(lastFolder).exists()) {
        currentFolderPath = lastFolder;

        QDir dir(lastFolder);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";
        QStringList fileList = dir.entryList(filters, QDir::Files);
        ui->comboBox->clear();
        ui->comboBox->addItems(fileList);
    }
}

void MainWindow::saveLastFolder()
{
    settings->setValue("LastFolder", currentFolderPath);
    settings->sync();
}

void MainWindow::on_pushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку с изображениями",
        currentFolderPath.isEmpty() ? QDir::homePath() : currentFolderPath
        );

    if (folderPath.isEmpty()) {
        return;
    }

    currentFolderPath = folderPath;
    QDir dir(folderPath);

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";
    QStringList fileList = dir.entryList(filters, QDir::Files);

    ui->comboBox->clear();
    ui->comboBox->addItems(fileList);

    saveLastFolder();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == -1 || currentFolderPath.isEmpty()) {
        return;
    }

    QString fileName = ui->comboBox->itemText(index);
    QString filePath = currentFolderPath + "/" + fileName;

    QPixmap pixmap(filePath);
    if (!pixmap.isNull()) {
        ui->label->setPixmap(pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->label->setText("Не удалось загрузить изображение");
    }
}
