#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framelesswindow.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QProcess>
#include <QWindow>

#include <QSettings>
#include <QStandardPaths>

#include "./components/controller.h"
#include "./components/firstmenu.h"
#include "./components/secondmenu.h"
#include "./components/message.h"
#include "./components/mssa_update.h"
#include "./components/aboutsoftwave.h"
#include "./pages/home/homepage.h"
#include "./pages/dashboard/dashboard.h"
#include "./pages/goodslist/goodslist.h"
#include "./pages/categoryinform/categoryinform.h"
#include "./pages/order/orderlist.h"
#include "./pages/import/import.h"
#include "./pages/analysis/summary.h"
#include "./pages/analysis/sales.h"
#include "./pages/analysis/list.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public CFramelessWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isMax(){
        return maximized;
    }
    QString getStore(){
        return store;
    }
    QString getBrand(){
        return brand;
    }

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void controlWindowScale();
    void setFirstMenu(QString page);

    void addTryIcon();
    void openGl();
    void readSet();
    void addUpdater();
    void addAbout();
    void addSettings();
    void addFirstMenu();
    void addSecondMenu();
    void addController();
    void addPageStacked();
    void addMessageBar();

    void refreshPages();
    QWidget *createProcess(QString exePath, QString idTag);

protected slots:
    void addPage(const QString &name, const QString &parent);
    void activeTray(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;

    // 版本信息
    int version_id = 10105;
    QString version_name = "1.0.5";
    QString version_type = "release";

    // 内容组件
    mssa_update *updater;
    AboutSoftwave *about;
    QSystemTrayIcon* trayIcon;
    QMenu *systemTrayIconMenu;
    Message *message;
    MenuFirstLevel *menuFirstLevel;
    SecondMenu *secondMenu;
    controller *controls;
    QVBoxLayout *content_layout;
    QStackedWidget *content_staked;

    // 窗口控制
    QPoint clickPos;
    bool maximized = false;
    QRect lastGeometry;
    const qint64 moveLimitY = 40;
    const qint64 moveLimitX = 60;
    const qint64 moveLimitYB = 24;
    bool doNotRecordWindowSize = false;

    // 内容配置
    QSettings *settings;
    QString serve;
    QString store;
    QString brand;
    QFont iconfont;
    QString first_menu = "home";
    QString second_menu = "home";

    // 页面信息
    bool secondMenuAdded = false;
    qint64 pageLoaded = 0;
    int homepageLoaded = false;
    HomePage *home;
    int dashboardLoaded = false;
    dashboard *dash;
    int goodslistLoaded = false;
    goodslist *goods_list;
    int categoryLoaded = false;
    categoryinform *category;
    int orderlistLoaded = false;
    OrderList *order;
    int importPageLoaded = false;
    ImportPage *import;
    int summaryPageLoaded = false;
    SummaryPage *summary;
    int salesAnalysisLoaded = false;
    SalesAnalysis *sales;
    int salesListLoaded = false;
    SalesAndRefundsListPage *list;
};
#endif // MAINWINDOW_H
