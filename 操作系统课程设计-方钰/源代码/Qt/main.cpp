#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen> //开机动画
#include <QTextCodec>//汉字编码

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //设置中文编码
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    MainWindow window;
    QSplashScreen splash(QPixmap(":/Resources/splash.png"));

    splash.show();//动画窗口
    for (int i = 0;i < 10000000;i++);
    splash.finish(&window);

    window.show();//主窗口
    window.setWindowIcon(QIcon(":/Resources/tju.png"));
    return app.exec();
}
