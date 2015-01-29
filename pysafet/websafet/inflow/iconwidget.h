#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>
#include <QLabel>



class IconWidget : public QLabel
{
Q_OBJECT
public:
    IconWidget(QWidget *parent = 0);

    QSize sizeHint() const;

    void setMessageAndImage(QString nameOfImage);
    void setTextOfLabel(QString text);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    // label
    QLabel * label;

public slots:

};

#endif // ICONWIDGET_H
