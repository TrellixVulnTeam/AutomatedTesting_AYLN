#ifndef SNDIALOG_H
#define SNDIALOG_H

#include <QDialog>
#include "SNInputForm.h"
#include "UnitsForm.h"

class UnitsForm;

namespace Ui
{
class SNDialog;
}

class SNDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SNDialog(QWidget* parent = 0, qint16 _uutCount = 1);
    ~SNDialog();
    void execDialog(QList<UnitsForm*> forms);
    void clearSN(qint16 slot);

signals:
    void startAll(const QList<QString>& list);
    void singleStart(qint16 slot, const QString& sn);

private slots:
    void on_allStartBtn_clicked();
    void onReturnPressed(qint16 slot);

private:
    Ui::SNDialog* ui;
    qint16 uutCount;
    QList<SNInputForm*> snForms;
    QList<UnitsForm*> unitForms;
};

#endif  // SNDIALOG_H
