#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Infile(nullptr),
    m_Key(nullptr),
    m_Outfile(nullptr)
{
    ui->setupUi(this);
    // AES_init_ctx(&ctx, key);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString strKeyPath = QFileDialog::getOpenFileName(this, "KeyFile", "", "keyFile (*.*)");
    QFile file(strKeyPath);
    if(file.exists() && file.open(QIODevice::ReadOnly))
    {
        ui->lineEdit->setText(strKeyPath);
        m_Key = file.readAll();
        AES_init_ctx(&ctx, (unsigned char*)(m_Key.data()));
        file.close();
    }else{
        qDebug() << "Key File open failed!";
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    m_strInputPath = QFileDialog::getOpenFileName(this, "InFile", "", "InFile (*.*)");
    QFile file(m_strInputPath);
    if(file.exists() && file.open(QIODevice::ReadOnly))
    {
        ui->lineEdit_2->setText(m_strInputPath);
        m_Infile = file.readAll();
        file.close();
    }else{
        qDebug() << "Key File open failed!";
        m_strInputPath = "";
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(false == m_strInputPath.isEmpty() &&
//            nullptr!=m_Key &&
            nullptr!=m_Infile)
    {
        QString strKey = ui->lineEdit->text();
        if(true == strKey.isEmpty())
        {
            QMessageBox::warning(this, QString("warning"), QString("Please input Key"), QMessageBox::Ok);
            return;
        }
        m_Key = strKey.toLatin1();
        AES_init_ctx(&ctx, (unsigned char*)(m_Key.data()));
        int nInputSize = m_Infile.size();

        uint8_t *in = (uint8_t*)malloc(sizeof(char)*nInputSize);
        memset(in, 0, nInputSize);
        strncpy((char*)in, (char*)m_Infile.data(), nInputSize);
        AES_ECB_encrypt(&ctx, in);

        QFileInfo fileInputInfo(m_strInputPath);
        QString strOutFileName = fileInputInfo.baseName() + "_out.bin";
        QString strOutFilePath = fileInputInfo.path() + "/" + strOutFileName;

        QFile fileOut(strOutFilePath);
        if(fileOut.open(QIODevice::ReadWrite))
        {
            fileOut.write((char*)in, nInputSize);
            fileOut.flush();
            fileOut.close();
            ui->lineEdit_3->setText(strOutFilePath);
        }

    }

}
