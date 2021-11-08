#include "FileWatcherDialog.h"
#include "ui_FileWatcherDialog.h"
#include "User.h"
#include <QMessageBox>

FileWatcherDialog::FileWatcherDialog(const QJsonArray& fileList, QSettings* setting, bool* isok, QWidget* parent)
    : m_setting(setting), m_isok(isok), QDialog(parent), ui(new Ui::FileWatcherDialog)
{
    ui->setupUi(this);
    setWindowTitle("Watcher");
    ui->passwordLine->setEchoMode(QLineEdit::Password);

    ui->tableWidget->setColumnWidth(0, this->width());
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    foreach (QJsonValue jsonData, fileList) {
        QTableWidgetItem* noItem = new QTableWidgetItem(jsonData.toString());
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, noItem);
    }
}

FileWatcherDialog::~FileWatcherDialog()
{
    delete ui;
}

void FileWatcherDialog::on_affirmBtn_clicked()
{
    QVariantMap value = m_setting->value(DEVELOPER).toMap();

    if (value.value(PASSWD).toString()
        == QCryptographicHash::hash(ui->passwordLine->text().toUtf8(), QCryptographicHash::Md5).toHex()) {
        QMessageBox::information(this, "Hint", "File updated successfully");
        close();
        *m_isok = true;
    } else {
        QMessageBox::warning(this, "Warning", "Incorrect password, please try again");
        *m_isok = false;
    }
}

void FileWatcherDialog::on_cancelBtn_clicked()
{
    close();
    *m_isok = false;
}

void FileWatcherDialog::closeEvent(QCloseEvent*)
{
    *m_isok = false;
}
