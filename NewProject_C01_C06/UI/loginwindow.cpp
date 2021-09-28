#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QAction* tempAction, QWidget* parent) : QWidget(parent), ui(new Ui::LoginWindow)
{

    ui->setupUi(this);
    ui->textEdit_Users->setAlignment(Qt::AlignVCenter);
    ui->textEdit_Password->setAlignment(Qt::AlignVCenter);
    this->tempAction = tempAction;
    setWindowTitle("Login");
    if (tempAction->text() == "Engineer") {

        ui->textEdit_Users->setText("Engineer");
    }
    if (tempAction->text() == "OP") {
        ui->textEdit_Users->setText("OP");
    }
    if (tempAction->text() == "TE") {
        ui->textEdit_Users->setText("TE");
    }

    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(cancelButton_trigger()));
    connect(ui->pushButton_login, SIGNAL(clicked(bool)), this, SLOT(loginButton_trigger()));
}

void LoginWindow::cancelButton_trigger()
{

    this->close();
}

void LoginWindow::loginButton_trigger()
{
    if (ui->textEdit_Users->text() == "Engineer") {

        if (ui->textEdit_Password->text() == "SW123") {

            this->close();
        } else {
            QMessageBox::warning(NULL, QStringLiteral("Waring"), QStringLiteral("Wrong password, please re-enter"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    } else if (ui->textEdit_Users->text() == "OP") {

        if (ui->textEdit_Password->text() == "OP123") {

            this->close();
        } else {
            QMessageBox::warning(NULL, QStringLiteral("Waring"), QStringLiteral("Wrong password, please re-enter"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }

    } else if (ui->textEdit_Password->text() == "TE") {

        if (ui->textEdit_Password->text() == "TE123") {

            this->close();
        } else {
            QMessageBox::warning(NULL, QStringLiteral("Waring"), QStringLiteral("Wrong password, please re-enter"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    } else {

        QMessageBox::warning(NULL, QStringLiteral("Waring"), QStringLiteral("The current user does not exist"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
