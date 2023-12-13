#include "mainwindow.h"
#include "loginwindow.h"

#include <QtGui>
#include <windows.h>
#include <winuser.h>

#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>

void setSurfaceFont(){
    QString fontPath = ":/font/PingFang Medium.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    QString fontFamily = fontFamilies.at(0);
    QFont font(fontFamily, 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(font);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString name = "Qt5QWindowIcon";
    QString windowname = QStringLiteral("喵街 数据整理汇总与分析 - 万寻");
    HWND hwnd = FindWindow((LPCWSTR)name.unicode(),(LPCWSTR)windowname.unicode());
    if(hwnd){
        ShowWindow(hwnd, SW_RESTORE);
        BringWindowToTop(hwnd);
        return 0;
    }

    setSurfaceFont();

    LoginWindow l;
    l.show();

    return a.exec();
}
