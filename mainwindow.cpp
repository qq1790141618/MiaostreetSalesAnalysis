#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"

#include <QTimer>
#include <QSurfaceFormat>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QStyle>

#include "./components/settingwindow.h"
#include "./components/closeconfirm.h"

MainWindow::MainWindow(QWidget *parent)
    : CFramelessWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();

    addUpdater();
    addAbout();
    addTryIcon();
    openGl();
    readSet();

    addFirstMenu();
    addSecondMenu();
    addController();
    addPageStacked();
    addMessageBar();

    secondMenu->mackMenu(first_menu, "menu");
    updater->checkUpdate(serve, version_id, version_name, version_type);
    menuFirstLevel->checkSecondMenuSet();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openGl(){
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    QSurfaceFormat::setDefaultFormat(format);

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

void MainWindow::readSet(){
    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
    serve = "https://work-serve.fixeam.com/api";
    store = "132";
    brand = QStringLiteral("兔皇");
    iconfont = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/font/iconfont.ttf")).at(0);

    settings->beginGroup("SETTINGS");
    QString localStore = settings->value("STORE_ID", "").toString();
    if(localStore != ""){
        store = localStore;
    }
    QString localBrand = settings->value("BRAND", "").toString();
    if(localBrand != ""){
        brand = localBrand;
    }

    qint64 windowWidth = settings->value("WINDOW_WIDTH", 0).toInt();
    qint64 windowHeight = settings->value("WINDOW_HEIGHT", 0).toInt();
    if(windowWidth != 0 && windowHeight != 0){
        resize(windowWidth, windowHeight);
    }else{
        resize(1600, 900);
    }
    settings->endGroup();
}

void MainWindow::addUpdater(){
    updater = new mssa_update();
    connect(updater, &mssa_update::updatelaunch, this, [=]{
        doNotRecordWindowSize = true;
        updater->close();
        close();
    });
}

void MainWindow::addAbout(){
    about = new AboutSoftwave();
    about->setVersionName(version_name + " " + version_type);
    connect(about, &AboutSoftwave::checkUpdate, this, [=]{
        updater->show();
        updater->setFixedSize(QSize(500, 350));
        updater->setUserClick(true);
        updater->checkUpdate(serve, version_id, version_name, version_type);
    });
}

void MainWindow::addSettings(){
    SettingWindow *settingWindow = new SettingWindow();
    settingWindow->show();

    connect(settingWindow, &SettingWindow::logout, this, [=]{
        LoginWindow *login = new LoginWindow();
        login->show();
        trayIcon->hide();
        close();
    });
    connect(settingWindow, &SettingWindow::changeSetting, this, [=](QString st, QString br){
        if(store != st || brand != br){
            store = st;
            brand = br;
            refreshPages();

            message->setIcon(QPixmap(":/images/colorful/company.png"));
            QString storeShow;
            if(store == "132"){
                storeShow = QStringLiteral("海宁银泰城");
            }
            if(store == "HZ13"){
                storeShow = QStringLiteral("杭州文化广场");
            }
            if(store == "HZ20"){
                storeShow = QStringLiteral("杭州滨江银泰广场");
            }
            message->setText(QStringLiteral("已经成功切换门店 商场: ") + storeShow + QStringLiteral(" 品牌: ") + brand);
        } else {
            message->setIconFont(QChar(0xe645));
            message->setText(QStringLiteral("设置保存成功"));
        }
    });
}

void MainWindow::addTryIcon(){
    trayIcon = new QSystemTrayIcon(QIcon("://images/favicon.ico"));

    systemTrayIconMenu = new QMenu(this);
    systemTrayIconMenu->setAttribute(Qt::WA_TranslucentBackground);
    systemTrayIconMenu->setWindowFlags(systemTrayIconMenu->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    systemTrayIconMenu->setCursor(Qt::PointingHandCursor);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#a6a6a6"));
    shadow->setBlurRadius(6);
    systemTrayIconMenu->setGraphicsEffect(shadow);
    systemTrayIconMenu->setStyleSheet("QMenu { border-radius: 7px; background-color: #ffffff; padding: 7px 1px; margin: 5px; }\
                                QMenu::item{ padding: 6px 36px; font-size: 9pt; }\
                                QMenu::item:selected { background-color: #f3f3f3; }");

    QAction *systemTrayIconMenuShow = new QAction(systemTrayIconMenu);
    systemTrayIconMenuShow->setText(QStringLiteral("显示主界面"));
    connect(systemTrayIconMenuShow, &QAction::triggered, this, [=]{
        HWND hWnd = (HWND)this->winId();
        ShowWindow(hWnd, SW_SHOW);
    });
    systemTrayIconMenu->addAction(systemTrayIconMenuShow);

    QAction *systemTrayIconMenuSet = new QAction(systemTrayIconMenu);
    systemTrayIconMenuSet->setText(QStringLiteral("设置"));
    connect(systemTrayIconMenuSet, &QAction::triggered, this, [=]{
        addSettings();
    });
    systemTrayIconMenu->addAction(systemTrayIconMenuSet);

    QAction *systemTrayIconMenuAbout = new QAction(systemTrayIconMenu);
    systemTrayIconMenuAbout->setText(QStringLiteral("关于软件"));
    connect(systemTrayIconMenuAbout, &QAction::triggered, this, [=]{
        about->show();
    });
    systemTrayIconMenu->addAction(systemTrayIconMenuAbout);

    QAction *systemTrayIconMenuExit = new QAction(systemTrayIconMenu);
    systemTrayIconMenuExit->setText(QStringLiteral("退出"));
    connect(systemTrayIconMenuExit, &QAction::triggered, this, [=]{
        close();
    });
    systemTrayIconMenu->addAction(systemTrayIconMenuExit);

    trayIcon->setContextMenu(systemTrayIconMenu);
    trayIcon->setVisible(true);
    trayIcon->setToolTip("Miaostreet Sales Analysis Tool");
    trayIcon->show();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::activeTray);
}

void MainWindow::activeTray(QSystemTrayIcon::ActivationReason reason){
    HWND hWnd = (HWND)this->winId();

    switch (reason) {
    case QSystemTrayIcon::Context:
        systemTrayIconMenu->show();
        break;
    case QSystemTrayIcon::DoubleClick:
        ShowWindow(hWnd, SW_SHOW);
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    case QSystemTrayIcon::Trigger:
        show();
        break;
    default:
        break;
    }
}

void MainWindow::addFirstMenu(){
    menuFirstLevel = new MenuFirstLevel(this);
    ui->horizontalLayout_main->addWidget(menuFirstLevel);
    connect(menuFirstLevel, &MenuFirstLevel::aboutShow, this, [=]{
        about->show();
    });
    connect(menuFirstLevel, &MenuFirstLevel::settingsShow, this, [=]{
        addSettings();
    });
    connect(menuFirstLevel, &MenuFirstLevel::exit, this, [=]{
        close();
    });
    connect(menuFirstLevel, &MenuFirstLevel::min, this, [=]{
        showMinimized();
    });
    connect(menuFirstLevel, &MenuFirstLevel::max, this, [=]{
        controlWindowScale();
    });
    connect(menuFirstLevel, &MenuFirstLevel::changeFirstMenu, this, &MainWindow::setFirstMenu);
    connect(menuFirstLevel, &MenuFirstLevel::mackSecondMenu, this, [=](QString page){
        if(secondMenuAdded){
            first_menu = page;
            secondMenu->mackMenu(page, "menu");
        }
    });
    connect(menuFirstLevel, &MenuFirstLevel::logout, this, [=]{
        LoginWindow *login = new LoginWindow();
        login->show();
        trayIcon->hide();
        close();
    });
    connect(menuFirstLevel, &MenuFirstLevel::hideSecondMenu, this, [=](bool status){
        if(status){
            secondMenu->hide();
        } else {
            secondMenu->show();
        }
    });
    connect(menuFirstLevel, &MenuFirstLevel::addPage, this, &MainWindow::addPage);
}

void MainWindow::setFirstMenu(QString page){
    first_menu = page;
}

void MainWindow::addSecondMenu(){
    secondMenu = new SecondMenu(this);
    ui->horizontalLayout_main->addWidget(secondMenu);
    secondMenuAdded = true;
    connect(secondMenu, &SecondMenu::changeSecondMenu, this, &MainWindow::addPage);
}

QWidget *MainWindow::createProcess(QString exePath, QString idTag){
    QProcess *ex = new QProcess(this);
    ex->start(exePath);
    ex->waitForFinished(100);

    QString strExeFilePath = "G:/dddd/SettingInfo.ini";
    QSettings settings(strExeFilePath, QSettings::IniFormat);
    settings.beginGroup("Process");
    int ue = settings.value(idTag).toInt();
    settings.endGroup();

    QWindow *win = QWindow::fromWinId(ue);
    if (win){
        win->setFlags(win->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        QWidget *homeWidget = QWidget::createWindowContainer(win, this, Qt::Widget);
        return homeWidget;
    }
}

void MainWindow::refreshPages(){
    if(homepageLoaded){
        home->setParam(serve, store, brand);
    }
    if(dashboardLoaded){
        dash->setParam(serve, store, brand);
    }
    if(goodslistLoaded){
        goods_list->setParam(serve, store, brand);
    }
    if(categoryLoaded){
        category->setParam(serve, store, brand);
    }
    if(orderlistLoaded){
        order->setParam(store, brand);
    }
    if(importPageLoaded){
        import->setParam(serve, store, brand);
    }
    if(summaryPageLoaded){
        summary->setParam(serve, store, brand);
    }
    if(salesListLoaded){
        list->setParam(serve, store, brand);
    }
}

void MainWindow::addPage(const QString &name, const QString &parent){
    secondMenu->setSecondMenu(name);
    menuFirstLevel->setActiveFirstMenus(parent);
    menuFirstLevel->setActiveMenus();

    if(name == "home"){
        if(!homepageLoaded){
            home = new HomePage(this);
            home->setParam(serve, store, brand);
            content_staked->addWidget(home);
            connect(home, &HomePage::open, this, [=](QString page){
                addPage(page, "import");
            });

            pageLoaded++;
            homepageLoaded = pageLoaded;
        }
        if(second_menu == name){
            home->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(homepageLoaded - 1);
    }
    if(name == "dashboard"){
        if(!dashboardLoaded){
            dash = new dashboard(this);
            dash->setParam(serve, store, brand);
            content_staked->addWidget(dash);

            pageLoaded++;
            dashboardLoaded = pageLoaded;
        }
        if(second_menu == name){
            dash->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(dashboardLoaded - 1);
    }
    if(name == "goods_list"){
        if(!goodslistLoaded){
            goods_list = new goodslist(this);
            goods_list->setParam(serve, store, brand);
            content_staked->addWidget(goods_list);

            pageLoaded++;
            goodslistLoaded = pageLoaded;
        }
        if(second_menu == name){
            goods_list->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(goodslistLoaded - 1);
    }
    if(name == "category_inform"){
        if(!categoryLoaded){
            category = new categoryinform(this);
            category->setParam(serve, store, brand);
            content_staked->addWidget(category);

            pageLoaded++;
            categoryLoaded = pageLoaded;
        }
        if(second_menu == name){
            category->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(categoryLoaded - 1);
    }
    if(name == "orders_list"){
        if(!orderlistLoaded){
            order = new OrderList(this);
            order->setParam(store, brand);
            connect(order, &OrderList::openPage, this, [=](QString page){
                addPage(page, "import");
            });
            content_staked->addWidget(order);

            pageLoaded++;
            orderlistLoaded = pageLoaded;
        }
        content_staked->setCurrentIndex(orderlistLoaded - 1);
    }
    if(name == "import"){
        if(!importPageLoaded){
            import = new ImportPage(this);
            import->setParam(serve, store, brand);
            content_staked->addWidget(import);

            pageLoaded++;
            importPageLoaded = pageLoaded;
        }
        if(second_menu == name){
            import->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(importPageLoaded - 1);
    }
    if(name == "summary"){
        if(!summaryPageLoaded){
            summary = new SummaryPage(this);
            summary->setParam(serve, store, brand);
            content_staked->addWidget(summary);

            pageLoaded++;
            summaryPageLoaded = pageLoaded;
        }
        if(second_menu == name){
            summary->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(summaryPageLoaded - 1);
    }
    if(name == "analysis_task"){
        if(!salesAnalysisLoaded){
            sales = new SalesAnalysis(this);
            sales->setParam(serve, store, brand);
            content_staked->addWidget(sales);

            pageLoaded++;
            salesAnalysisLoaded = pageLoaded;
        }
        content_staked->setCurrentIndex(salesAnalysisLoaded - 1);
    }
    if(name == "analysis_refunds"){
        if(!salesListLoaded){
            list = new SalesAndRefundsListPage(this);
            list->setParam(serve, store, brand);
            content_staked->addWidget(list);

            pageLoaded++;
            salesListLoaded = pageLoaded;
        }
        if(second_menu == name){
            list->setParam(serve, store, brand);
        }
        content_staked->setCurrentIndex(salesListLoaded - 1);
    }
    second_menu = name;
}

void MainWindow::addController(){
    QWidget *content = new QWidget(this);
    ui->horizontalLayout_main->addWidget(content);
    content_layout = new QVBoxLayout(content);
    content_layout->setContentsMargins(0, 0, 0, 0);
    content_layout->setSpacing(0);

    controls = new controller(this);
    content_layout->addWidget(controls);
    connect(controls, &controller::min, this, [=]{
        showMinimized();
    });
    connect(controls, &controller::max, this, &MainWindow::controlWindowScale);
    connect(controls, &controller::close, this, [=]{
        settings->beginGroup("SETTINGS");
        int closeButtonTip = settings->value("MAINWINDOW_CLOSE_BUTTON_MESSAGE", 0).toInt();

        if(closeButtonTip == 1){
            QString closeButton = settings->value("MAINWINDOW_CLOSE_BUTTON", "hide").toString();

            if(closeButton == "hide"){
                HWND hWnd = (HWND)this->winId();
                ShowWindow(hWnd, SW_HIDE);
            } else {
                doNotRecordWindowSize = true;
                close();
            }
        } else {
            CloseConfirm *closeConfirm = new CloseConfirm();
            connect(closeConfirm, &CloseConfirm::confirm, this, [=](QString behavior){
                if(behavior == "hide"){
                    HWND hWnd = (HWND)this->winId();
                    ShowWindow(hWnd, SW_HIDE);
                } else {
                    doNotRecordWindowSize = true;
                    close();
                }
            });
            closeConfirm->show();
        }

        settings->endGroup();
    });
}

void MainWindow::addPageStacked(){
    content_staked = new QStackedWidget(this);
    content_layout->addWidget(content_staked);
    content_layout->setStretch(1, 1);
//    content_layout->setStretch(0, 1);
    addPage("home", "home");
}

void MainWindow::addMessageBar(){
    message = new Message(this);
    content_layout->addWidget(message);
    message->setText(QStringLiteral("点击设置可以切换门店/品牌"));
    message->setIcon(QPixmap(":/images/colorful/create.png"));
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QSize size = event->size();

    if(!doNotRecordWindowSize){
        settings->beginGroup("SETTINGS");
        settings->setValue("WINDOW_WIDTH",size.width());
        settings->setValue("WINDOW_HEIGHT",size.height());
        settings->endGroup();
    }

    QWidget::resizeEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
    if(maximized){
        controlWindowScale();
    }
    if(e->buttons()&Qt::LeftButton)
        if (clickPos.x() <= moveLimitX || clickPos.y() <= moveLimitY || clickPos.y() >= geometry().height() - moveLimitYB) {
            move(e->pos() + pos() - clickPos);
        }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e){
    if (e->button() == Qt::LeftButton)
        if (clickPos.x() <= moveLimitX || clickPos.y() <= moveLimitY || clickPos.y() >= geometry().height() - moveLimitYB) {
            QTimer::singleShot(120, this, &MainWindow::controlWindowScale);
        }
}

void MainWindow::controlWindowScale(){
#ifdef Q_OS_WINDOWS
    if (!maximized) {
        doNotRecordWindowSize = true;
        lastGeometry = frameGeometry();

        showMaximized();
        maximized = true;
        menuFirstLevel->setMax(maximized);
//        controls->setmaxIcon(!maximized);
    } else {
        showNormal();

        resize(lastGeometry.width(), lastGeometry.height());
        move(lastGeometry.x(), lastGeometry.y());
        maximized = false;
        menuFirstLevel->setMax(maximized);
//        controls->setmaxIcon(!maximized);

        doNotRecordWindowSize = false;
    }
#endif
}
