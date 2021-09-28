#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMessageBox>

namespace Ui
{
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QAction* tempAction, QWidget* parent = 0);
    ~LoginWindow();

public slots:
    void cancelButton_trigger();
    void loginButton_trigger();

private:
    Ui::LoginWindow* ui;
    QAction* tempAction;
};

#endif  // LOGINWINDOW_H
