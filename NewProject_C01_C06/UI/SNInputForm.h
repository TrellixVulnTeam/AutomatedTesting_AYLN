#ifndef SNINPUTFORM_H
#define SNINPUTFORM_H

#include <QWidget>

namespace Ui
{
class SNInputForm;
}

class SNInputForm : public QWidget
{
    Q_OBJECT

public:
    explicit SNInputForm(QWidget* parent = 0, qint16 _slot = 1);
    ~SNInputForm();

    void setSNFocus();
    QString getSN() const;
    void clearSN();

signals:
    void singleStart(qint16 slot, const QString& sn, const std::string& mtcpFilePath);
    void returnPressed(qint16 slot);

private slots:
    void on_singleStartBtn_clicked();

private:
    Ui::SNInputForm* ui;
    qint16 slot;
};

#endif  // SNINPUTFORM_H
