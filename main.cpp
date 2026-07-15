#include "components/mainwindow.h"

#include <QApplication>

#include "ElaApplication.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    // 启用高 DPI 像素图支持（Qt 5.x）
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    // Qt 5.14+ 启用高 DPI 缩放
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // 设置高 DPI 缩放因子舍入策略为直通，避免模糊
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
    // Qt 5.14 以下版本手动设置缩放因子
    // 根据实际屏幕缩放比例更改此值
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#endif

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resource/image/logo/Ocean.png"));

    eApp->init();

    MainWindow w;

    return a.exec();
}
