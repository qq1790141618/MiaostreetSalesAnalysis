
#ifndef INBROWSER_H
#define INBROWSER_H

#include <QMainWindow>

#include <QSettings>
#include <QWebEngineView>
#include <QMouseEvent>

namespace Ui {
class inbrowser;
}

class inbrowser : public QMainWindow
{
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    explicit inbrowser(QWidget *parent = nullptr);
    ~inbrowser();
    void setSize(int width, int height){
        resize(width, height);
    }
    void setUrl(QString url);

private slots:
    void onPageLoadFinished(bool ok);
    void onPageUrlChanged(const QUrl& url);
    void handleIconUrlChanged(const QUrl &url);
    void on_pushButton_more_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_next_clicked();

private:
    Ui::inbrowser *ui;

    QPoint clickPos;
    bool maximized = false;
    QRect lastGeometry;
    QUrl mainUrl;
    int mainIndex;
    int maxIndex;
    QSettings *settings;

    QWebEngineView *webview;
    bool record = true;
};

#endif // INBROWSER_H
