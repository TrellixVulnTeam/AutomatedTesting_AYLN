#ifndef HTMLDIALOG_H
#define HTMLDIALOG_H

#include <QDialog>
#include <QWebEngineView>
#include <QPushButton>
#include <QJsonArray>
#include <QSettings>
#include "ZmqRepProtocol.h"

namespace Ui
{
class HtmlDialog;
}

class HtmlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlDialog(QSettings* setting, QWidget* parent = 0);
    ~HtmlDialog();

    static std::string zmqRepCallBack(void* buf, long len, void* context);

signals:
    void showWaring(const QString& msg);
    void showFileWatcherDialig(const QJsonArray& jsonArr, bool* isok);
    void watcherFileTrigger();

private slots:
    void onLeftBtnClicked();
    void onRrightBtnClicked();

    void onShowWaring(const QString& msg);
    void onShowFileWatcherDialig(const QJsonArray& jsonArr, bool* isok);

private:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void showEvent(QShowEvent*);

private:
    Ui::HtmlDialog* ui;

    QWebEngineView* m_webView = NULL;
    QPushButton* m_leftBtn = NULL;
    QPushButton* m_rightBtn = NULL;
    DynamicZmqRep* m_zmqRep = NULL;
    QSettings* m_setting = NULL;

    std::vector<std::string> m_files;
    int m_currIdx = 0;
};

#endif  // HTMLDIALOG_H
