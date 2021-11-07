#ifndef HTMLDIALOG_H
#define HTMLDIALOG_H

#include <QDialog>
#include <QWebEngineView>
#include <QPushButton>

namespace Ui
{
class HtmlDialog;
}

class HtmlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlDialog(QWidget* parent = 0);
    ~HtmlDialog();

private slots:
    void leftBtnClicked();
    void rightBtnClicked();
    void rollbackBtnClicked();

private:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    Ui::HtmlDialog* ui;

    QWebEngineView* m_webView = NULL;
    QPushButton* m_leftBtn = NULL;
    QPushButton* m_rightBtn = NULL;
};

#endif  // HTMLDIALOG_H
