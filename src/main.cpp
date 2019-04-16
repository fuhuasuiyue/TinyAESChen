#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMutex>
#include <QDateTime>

extern "C" void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }
    /* The QMessageLogContext class provides additional information about a log message.
    The class provides information about the source code location a qDebug(), qInfo(), qWarning(), qCritical() or qFatal() message was generated.
    Note: By default, this information is recorded only in debug builds. You can overwrite this explicitly by defining QT_MESSAGELOGCONTEXT or QT_NO_MESSAGELOGCONTEXT
    */
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(current_date).arg(msg);

    QDir dirLog = QCoreApplication::applicationDirPath();
#ifdef Q_OS_MACOS
    dirLog.cdUp();
#endif
    // dirLog.cdUp();
    QString strLogPath = dirLog.absolutePath() + QString("/log");
    if(!dirLog.exists(strLogPath))
    {
        dirLog.mkdir(strLogPath);
    }
    dirLog.cd("log");
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString strLogFile = dirLog.absoluteFilePath(QString("log_")+currentTime+QString(".txt"));
    QFile file(strLogFile);
    file.open(QIODevice::WriteOnly| QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream.setCodec("utf-8");
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(outputMessage);
    // 如果双击软件或者命令行就一个命令就显示界面
    MainWindow w;
    if(argc == 1)
    {
        w.show();
    }
    else{
        if(argc == 3)
        {
//            QString strKeyFileName = argv[1];
//            strKeyFileName = QDir::toNativeSeparators(strKeyFileName);
//            qDebug() << "strKeyFileName: " << strKeyFileName;
            QString strKey = argv[1];
            QString strInputFileName = argv[2];
            strInputFileName = QDir::toNativeSeparators(strInputFileName);
            qDebug() << "strInputFileName: " << strInputFileName;

            // QFile fileKey(strKeyFileName);
            QFile fileIn(strInputFileName);

            if(/*fileKey.exists() && */fileIn.exists())
            {
                // fileKey.open(QIODevice::ReadOnly);
                fileIn.open(QIODevice::ReadOnly);
                QByteArray baKey = strKey.toLatin1();
                QByteArray baIn = fileIn.readAll();
                struct AES_ctx ctx;
                AES_init_ctx(&ctx, (unsigned char*)(baKey.data()));

                int nInputSize = baIn.size();

                uint8_t *in = (uint8_t*)malloc(sizeof(char)*nInputSize);
                memset(in, 0, nInputSize);
                strncpy((char*)in, (char*)baIn.data(), nInputSize);
                AES_ECB_encrypt(&ctx, in);

                QFileInfo fileInputInfo(strInputFileName);
                QString strOutFileName = fileInputInfo.baseName() + "_out.bin";
                QString strOutFilePath = fileInputInfo.path() + "/" + strOutFileName;

                QFile fileOut(strOutFilePath);
                if(fileOut.open(QIODevice::ReadWrite))
                {
                    fileOut.write((char*)in, nInputSize);
                    fileOut.flush();
                    fileOut.close();
                }


            }
        }

    }


    return a.exec();
}
