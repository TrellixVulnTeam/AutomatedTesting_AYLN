#ifndef POSITIONDIALOG_H
#define POSITIONDIALOG_H

#include <QDialog>

namespace Ui
{
class PositionDialog;
}

class PositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PositionDialog(QWidget* parent = 0);
    ~PositionDialog();

private:
    Ui::PositionDialog* ui;
};

#endif  // POSITIONDIALOG_H
