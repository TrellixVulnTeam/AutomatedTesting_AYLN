#ifndef STARTLOADFORM_H
#define STARTLOADFORM_H

#include <QWidget>

class StartLoadForm : public QWidget
{
    Q_OBJECT
public:
    explicit StartLoadForm(QWidget* parent = nullptr);

signals:

public slots:

private:
    void paintEvent(QPaintEvent* event) override;

private:
    int m_num = 0;
};

#endif  // STARTLOADFORM_H
