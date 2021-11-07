#ifndef FILEWATCHERDIALOG_H
#define FILEWATCHERDIALOG_H

#include <QDialog>
#include <QJsonArray>
#include <QJsonValue>
#include <QSettings>

namespace Ui
{
class FileWatcherDialog;
}

class FileWatcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileWatcherDialog(const QJsonArray& fileList, QSettings* setting, bool* isok, QWidget* parent = 0);
    ~FileWatcherDialog();

private slots:
    void on_affirmBtn_clicked();
    void on_cancelBtn_clicked();

private:
    void closeEvent(QCloseEvent*);

private:
    Ui::FileWatcherDialog* ui;

    QSettings* m_setting = NULL;
    bool* m_isok = NULL;
};

#endif  // FILEWATCHERDIALOG_H
