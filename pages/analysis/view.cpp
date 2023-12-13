#include "view.h"
#include "ui_view.h"

#include <QStyle>
#include <QScreen>

view::view(QWidget *parent) :
    CFramelessWindow(parent),
    ui(new Ui::view)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();

    setAttribute(Qt::WA_DeleteOnClose);

    control = new controller(this);
    ui->horizontalLayout->addWidget(control);
    connect(control, &controller::min, this, [=]{
        showMinimized();
    });
    connect(control, &controller::max, this, &view::controlWindowScale);
    connect(control, &controller::close, this, [=]{
        close();
    });
    ui->horizontalLayout->setStretch(1, 1);

    webview = new QWebEngineView(this);
    webview->page()->setBackgroundColor(QColor("#f2f3ff"));
    connect(webview, &QWebEngineView::loadFinished, this, [=] {
        QString javascriptCode = QString("createPage('https://work-serve.fixeam.com/api', '%1')").arg(id);
        webview->page()->runJavaScript(javascriptCode);
    });
    ui->verticalLayout->addWidget(webview);

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->primaryScreen()->availableGeometry()
            )
    );
}

view::~view()
{
    delete ui;
}

void view::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void view::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton)
        if (clickPos.y() <= moveLimitY) {
            move(e->pos() + pos() - clickPos);
        }
}

void view::mouseDoubleClickEvent(QMouseEvent *e){
    if (e->button() == Qt::LeftButton)
        if (clickPos.y() <= moveLimitY) {
            QTimer::singleShot(120, this, &view::controlWindowScale);
        }
}

void view::controlWindowScale(){
#ifdef Q_OS_WINDOWS
    if (!maximized) {
        lastGeometry = frameGeometry();

        showMaximized();
        maximized = true;
        control->setmaxIcon(!maximized);
    } else {
        showNormal();

        resize(lastGeometry.width(), lastGeometry.height());
        move(lastGeometry.x(), lastGeometry.y());
        maximized = false;
        control->setmaxIcon(!maximized);
    }
#endif
}
