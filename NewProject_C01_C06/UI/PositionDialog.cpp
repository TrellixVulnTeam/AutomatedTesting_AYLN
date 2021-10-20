#include "PositionDialog.h"
#include "ui_PositionDialog.h"
#include <QDebug>
#include <QMessageBox>

PositionDialog::PositionDialog(QString positionStr, QWidget* parent)
    : m_positionStr(positionStr), QDialog(parent), ui(new Ui::PositionDialog)
{
    ui->setupUi(this);
    setStyleSheet("QDialog{min-height:25em;min-width:50em}");

    m_model = new QStandardItemModel(ui->tableView);
    connect(m_model, &QStandardItemModel::dataChanged, this, &PositionDialog::onDataChanged);
    ui->tableView->setModel(m_model);
    setTabViewModel();
    m_isDataChanged = false;
}

PositionDialog::~PositionDialog()
{
    delete ui;
}

void PositionDialog::setTabViewModel()
{
    m_model->clear();

    // The first line is the column name, and the first col is the row name
    QStringList tempr = m_positionStr.split("\n");
    QStringList rows;
    foreach (QString s, tempr) {
        if (s != "")
            rows.push_back(s);
    }

    int c = 0;
    for (int i = 0; i < rows.size(); i++) {
        QString rowStr = rows.at(i);
        QStringList cols = rowStr.split(",");

        if (i == 0) {
            QStringList valiCols;
            foreach (QString s, cols) {
                if (s != "")
                    valiCols.push_back(s);
            }
            c = valiCols.size() + 1;

            m_model->setColumnCount(c - 1);
            for (int j = 1; j < c; j++) {
                m_model->setHeaderData(j - 1, Qt::Horizontal, cols.at(j));
            }
            m_model->setRowCount(rows.size() - 1);
        } else {
            for (int j = 0; j < c; j++) {
                if (j == 0) {
                    m_model->setHeaderData(i - 1, Qt::Vertical, cols.at(0));
                } else {
                    m_model->setItem(i - 1, j - 1, new QStandardItem(cols.at(j)));
                }
            }
        }
    }
}

void PositionDialog::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight,
                                   const QVector<int>& roles)
{
    Q_UNUSED(bottomRight)
    Q_UNUSED(roles)

    // you can set limit for every slot
    if (m_model->data(topLeft).toString() == "") {
        m_isDataChanged = true;
        return;
    }

    float low = 0.0;
    float high = 0.0;
    if (topLeft.row() < 5) {
        low = -10;
        high = 10;
    } else if (topLeft.row() < 8) {
        low = -2;
        high = 5;
    } else if (topLeft.row() < 13) {
        low = -10;
        high = 10;
    } else if (topLeft.row() < 16) {
        low = -2;
        high = 5;
    } else if (topLeft.row() < 21) {
        low = -10;
        high = 10;
    } else if (topLeft.row() < 24) {
        low = -2;
        high = 5;
    }

    bool isOK = false;
    float value = m_model->data(topLeft).toFloat(&isOK);
    if (!isOK || value < low || value > high) {
        QBrush brush(QColor(255, 0, 0));
        if (m_model->item(topLeft.row(), topLeft.column())->foreground() != brush) {
            m_model->item(topLeft.row(), topLeft.column())->setForeground(brush);

            QMessageBox box(QMessageBox::Question, tr("Hint"),
                            tr("The value is not in the limit, whether to restore the original value?"),
                            QMessageBox::Yes | QMessageBox::Cancel);
            box.button(QMessageBox::Yes)->setText(tr("Yes"));
            box.button(QMessageBox::Cancel)->setText(tr("Cancel"));
            box.exec();
            if (box.clickedButton() == box.button(QMessageBox::Yes)) {
                QStringList rows = m_positionStr.split("\n");
                QString targetRowStr = rows.at(topLeft.row() + 1);
                QStringList cols = targetRowStr.split(",");
                QString targetValueStr = cols.at(topLeft.column() + 1);
                m_model->item(topLeft.row(), topLeft.column())->setText(targetValueStr);
                return;
            }
        }
    } else {
        m_model->item(topLeft.row(), topLeft.column())->setForeground(QBrush(QColor(0, 0, 0)));
    }

    m_isDataChanged = true;
}

void PositionDialog::on_saveBtn_clicked()
{
    if (m_isDataChanged) {
        int r = m_model->rowCount();
        int c = m_model->columnCount();
        QBrush brush(QColor(255, 0, 0));

        QString tabViewStr = ",";  // The first column in the first row is empty
        for (int i = 0; i < c; i++) {
            tabViewStr += m_model->headerData(i, Qt::Horizontal).toString();
            tabViewStr += ",";
        }
        tabViewStr += "\n";

        for (int i = 0; i < r; i++) {
            tabViewStr += m_model->headerData(i, Qt::Vertical).toString();
            tabViewStr += ",";
            for (int j = 0; j < c; j++) {
                QStandardItem* temp = m_model->item(i, j);
                if (NULL != temp) {
                    if (temp->text() != "" && temp->foreground() == brush) {
                        QMessageBox::warning(this, tr("Warning"),
                                             tr("Some data is invalid and cannot be saved. Please modify them."));
                        return;
                    }

                    tabViewStr += temp->text();
                    tabViewStr += ",";
                }
            }
            tabViewStr += "\n";
        }
        m_positionStr = tabViewStr;
        emit positionDataChanged(tabViewStr);
    }

    m_isDataChanged = false;
    hide();
}

void PositionDialog::on_cancelBtn_clicked()
{
    hide();
}
