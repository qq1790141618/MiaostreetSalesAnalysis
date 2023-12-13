#ifndef VIEW_H
#define VIEW_H

#include "framelesswindow.h"
#include "../../components/controller.h"

#include <QtWebEngineWidgets>

namespace Ui {
class view;
}

class view : public CFramelessWindow
{
    Q_OBJECT

public:
    explicit view(QWidget *parent = nullptr);
    ~view();
    void setId(const QString &taskId){
        id = taskId;
        webview->load(QUrl("qrc:/pages/analysis/view.html"));
    }

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void controlWindowScale();

private:
    Ui::view *ui;
    QString id;
    QPoint clickPos;
    bool maximized = false;
    QRect lastGeometry;
    const qint64 moveLimitY = 50;
    controller *control;
    QWebEngineView *webview;
};

#endif // VIEW_H
