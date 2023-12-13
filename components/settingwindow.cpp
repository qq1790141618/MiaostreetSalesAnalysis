#include "settingwindow.h"
#include "ui_settingwindow.h"

#include <QWidget>
#include <QListView>
#include <QStandardPaths>
#include <QPushButton>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>

#include "controller.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    initWindowStyle();
    addControl();
    readSet();
    initCombox();
    computerCache();
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::initWindowStyle(){
    ui->menubar->hide();
    ui->statusbar->hide();

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowModality(Qt::ApplicationModal);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#a6a6a6"));
    setGraphicsEffect(shadow);
}

void SettingWindow::addControl(){
    controller *setting_control = new controller(this);
    setting_control->onlyShowClose();
    setting_control->setTitle(QStringLiteral("设置"), QChar(0xe605));
    connect(setting_control, &controller::close, this, [=]{
        close();
    });
    ui->verticalLayout_set_control->addWidget(setting_control);
}

void SettingWindow::readSet(){
    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);

    settings->beginGroup("SETTINGS");

    QString localStore = settings->value("STORE_ID","").toString();
    if(localStore != ""){
        store = localStore;
    }
    if(store == "132"){
        ui->comboBox_store->setCurrentIndex(0);
    }
    if(store == "HZ13"){
        ui->comboBox_store->setCurrentIndex(1);
    }
    if(store == "HZ20"){
        ui->comboBox_store->setCurrentIndex(2);
    }

    QString localBrand = settings->value("BRAND","").toString();
    if(localBrand != ""){
        brand = localBrand;
    }
    if(brand == "KCOR"){
        ui->comboBox_brand->setCurrentIndex(0);
    }
    if(brand == "NT"){
        ui->comboBox_brand->setCurrentIndex(1);
    }
    if(brand == QStringLiteral("兔皇")){
        ui->comboBox_brand->setCurrentIndex(2);
    }
    if(brand == "DR"){
        ui->comboBox_brand->setCurrentIndex(3);
    }

    bool disautolog = settings->value("DISABLE_AUTO_LOG", false).toBool();
    if(!disautolog){
        ui->checkBox_autolog->setChecked(true);
    }

    int closeButtonMessage = settings->value("MAINWINDOW_CLOSE_BUTTON_MESSAGE", 1).toInt();
    closeButton = settings->value("MAINWINDOW_CLOSE_BUTTON", "hide").toString();
    if(closeButtonMessage == 0){
        ui->comboBox_close_button->setCurrentIndex(2);
    } else if(closeButton == "hide"){
        ui->comboBox_close_button->setCurrentIndex(1);
    } else {
        ui->comboBox_close_button->setCurrentIndex(0);
    }

    cache = settings->value("CACHE_STORAGE", "").toString();
    if(cache == ""){
        cache = "D:/MiaostreetSalesAnalysis/cache/";
        settings->setValue("CACHE_STORAGE", cache);
    }
    ui->label_fold->setText(cache);

    settings->endGroup();
}

void SettingWindow::initCombox(){
    ui->comboBox_store->installEventFilter(this);
    QListView *storelist = new QListView();
    ui->comboBox_store->setView(storelist);
    QWidget* containerObj = static_cast<QWidget*>(storelist->parent());
    containerObj->setWindowFlag(Qt::NoDropShadowWindowHint);
    containerObj->setStyleSheet("border: 1px solid #8eabff;");

    ui->comboBox_brand->installEventFilter(this);
    QListView *brandlist = new QListView();
    ui->comboBox_brand->setView(brandlist);
    QWidget* containerObj2 = static_cast<QWidget*>(brandlist->parent());
    containerObj2->setWindowFlag(Qt::NoDropShadowWindowHint);
    containerObj2->setStyleSheet("border: 1px solid #8eabff;");

    ui->comboBox_close_button->installEventFilter(this);
    QListView *cllist = new QListView();
    ui->comboBox_close_button->setView(cllist);
    QWidget* containerObj3 = static_cast<QWidget*>(cllist->parent());
    containerObj3->setWindowFlag(Qt::NoDropShadowWindowHint);
    containerObj3->setStyleSheet("border: 1px solid #8eabff;");
}

quint64 SettingWindow::dirFileSize(const QString &path)
{
    QDir dir(path);
    quint64 size = 0;

    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files)){
        size += fileInfo.size();
    }
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
        size += dirFileSize(path + QDir::separator() + subDir);
    }

    return size;
}

void SettingWindow::computerCache(){
    float size = dirFileSize(cache);
    float sizeMb = size / 1024 / 1024;
    ui->label_storage_size->setText(QString::number(sizeMb, 'f', 2) + "MB");
}

void SettingWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void SettingWindow::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton)
        move(e->pos() + pos() - clickPos);
}

bool SettingWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->comboBox_store || obj == ui->comboBox_brand || obj == ui->comboBox_close_button){
        if(event->type() == QEvent::MouseMove){
            return true;
        }
    }
    return QMainWindow::eventFilter(obj,event);
}

void SettingWindow::on_pushButton_shop_clicked()
{
    ui->pushButton_shop->setStyleSheet("border-right: 3px solid #b5c7ff;");
    ui->pushButton_base->setStyleSheet("");
    ui->pushButton_account->setStyleSheet("");
    ui->stacked_content->setCurrentIndex(0);
}

void SettingWindow::on_pushButton_base_clicked()
{
    ui->pushButton_base->setStyleSheet("border-right: 3px solid #b5c7ff;");
    ui->pushButton_shop->setStyleSheet("");
    ui->pushButton_account->setStyleSheet("");
    ui->stacked_content->setCurrentIndex(1);
}

void SettingWindow::on_pushButton_account_clicked()
{
    ui->pushButton_shop->setStyleSheet("");
    ui->pushButton_base->setStyleSheet("");
    ui->pushButton_account->setStyleSheet("border-right: 3px solid #b5c7ff;");
    ui->stacked_content->setCurrentIndex(2);
}

void SettingWindow::on_pushButton_exit_clicked()
{
    close();
}

void SettingWindow::on_pushButton_chooseFold_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择缓存目录"), cache, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isNull() && dir != cache){
        DeleteFileOrFolder(cache);
        cache = dir;

        QDir cdir;
        cdir.mkpath(dir);
        computerCache();

        settings->beginGroup("SETTINGS");
        settings->setValue("CACHE_STORAGE", cache);
        settings->endGroup();
    }
}

void SettingWindow::on_pushButton_storage_clicked()
{
    ui->pushButton_storage->setDisabled(true);
    ui->pushButton_storage->setText(QStringLiteral("清理中..."));

    QTimer::singleShot(120, this, [=]{
        DeleteFileOrFolder(cache);

        QTimer::singleShot(120, this, [=]{
            ui->pushButton_storage->setDisabled(false);
            ui->pushButton_storage->setText(QStringLiteral("清理缓存"));
            computerCache();
        });
    });
}

void SettingWindow::on_pushButton_save_clicked()
{
    int storeIndex = ui->comboBox_store->currentIndex();
    int brandIndex = ui->comboBox_brand->currentIndex();

    if(storeIndex == 0){
        store = "132";
    }
    if(storeIndex == 1){
        store = "HZ13";
    }
    if(storeIndex == 2){
        store = "HZ20";
    }
    if(brandIndex == 0){
        brand = "KCOR";
    }
    if(brandIndex == 1){
        brand = "NT";
    }
    if(brandIndex == 2){
        brand = QStringLiteral("兔皇");
    }
    if(brandIndex == 3){
        brand = "DR";
    }

    settings->beginGroup("SETTINGS");
    settings->setValue("STORE_ID", store);
    settings->setValue("BRAND", brand);
    settings->endGroup();

    emit changeSetting(store, brand);
    close();
}

bool SettingWindow::DeleteFileOrFolder(const QString &strPath)
{
    if (strPath.isEmpty() || !QDir().exists(strPath))
        return false;

    QFileInfo FileInfo(strPath);

    if (FileInfo.isFile())
        QFile::remove(strPath);
    else if (FileInfo.isDir())
    {
        QDir qDir(strPath);
        qDir.removeRecursively();
    }
    return true;
}

void SettingWindow::on_pushButton_logout_clicked()
{
    settings->beginGroup("SETTINGS");
    settings->setValue("DISABLE_LOG_OUT_TEMP", true);
    settings->endGroup();

    emit logout();
    close();
}

void SettingWindow::on_checkBox_autolog_stateChanged(int arg1)
{
    bool disautolog;

    if(arg1 == 0){
        disautolog = true;
    }
    if(arg1 == 2){
        disautolog = false;
    }

    settings->beginGroup("SETTINGS");
    settings->setValue("DISABLE_AUTO_LOG", disautolog);
    settings->endGroup();
}

void SettingWindow::on_comboBox_close_button_currentIndexChanged(int index)
{
    settings->beginGroup("SETTINGS");
    if(index == 0){
        settings->setValue("MAINWINDOW_CLOSE_BUTTON", "close");
    }
    if(index == 1){
        settings->setValue("MAINWINDOW_CLOSE_BUTTON", "hide");
    }
    if(index == 2){
        settings->setValue("MAINWINDOW_CLOSE_BUTTON_MESSAGE", 0);
    }
    settings->endGroup();
}

