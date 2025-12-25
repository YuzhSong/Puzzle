#include "server.h"
#include <QCoreApplication>  // 控制台应用程序

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 创建服务器对象
    server myServer;

    qDebug() << "Server started. Press Ctrl+C to exit.";

    return a.exec();
}
