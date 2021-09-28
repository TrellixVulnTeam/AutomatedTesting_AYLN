#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QTranslator>

namespace Ui
{
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget* parent = 0);
    ~AboutDialog();

public slots:
    void onClickLanguageAction(QTranslator& tran);

private:
    Ui::AboutDialog* ui;
private slots:
    void openUrl(const QString& url);

private:
    QString compilerString();
    QString buildCompatibilityString();
};

#endif  // ABOUTDIALOG_H
