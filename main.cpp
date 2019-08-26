#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QString>



void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    static QMutex mutex;
    mutex.lock();
    QString info_head;

    switch (type) {
    case QtDebugMsg:
        info_head = QString("Debug:");
        break;
    case QtInfoMsg:
        info_head = QString("Info:");
        break;
    case QtWarningMsg:
        info_head = QString("Warning:");
        break;
    case QtCriticalMsg:
        info_head = QString("Critical:");
        break;
    case QtFatalMsg:
        info_head = QString("Fatal:");
        break;
      }

    QString current_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString message = QString("%1" "%2" " --> %3").arg(info_head).arg(current_time).arg(msg);

    QFile file("Administrator.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();

}



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qInstallMessageHandler(myMessageOutput);//日志输出

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
