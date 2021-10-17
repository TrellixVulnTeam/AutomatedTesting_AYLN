#ifndef MOVEBTN_H
#define MOVEBTN_H

#include <QWidget>

namespace Ui
{
class MoveBtn;
}

class MoveBtn : public QWidget
{
    Q_OBJECT

public:
    explicit MoveBtn(QWidget* parent = 0);
    ~MoveBtn();

    QString getName() const;

private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

signals:
    void clicked();

public:
    void setBackground(const QString& cStr);
    void setSize(int w = 20, int h = 20);
    void setName(const QString& name);
    void setIcon(const QString& iconPath);

private:
    Ui::MoveBtn* ui;

    QString m_color = "gray";
};

#endif  // MOVEBTN_H
