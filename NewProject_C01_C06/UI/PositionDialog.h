#ifndef POSITIONDIALOG_H
#define POSITIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

class QStandardItemModel;

namespace Ui
{
class PositionDialog;
}

class PositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PositionDialog(QString positionStr, QWidget* parent = 0);
    ~PositionDialog();

signals:
    void positionDataChanged(const QString& data);

private slots:
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight,
                       const QVector<int>& roles = QVector<int>());

    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();

private:
    void setTabViewModel();

private:
    Ui::PositionDialog* ui;

    QString m_positionStr;
    QStandardItemModel* m_model = NULL;
    bool m_isDataChanged = false;
};

#endif  // POSITIONDIALOG_H
