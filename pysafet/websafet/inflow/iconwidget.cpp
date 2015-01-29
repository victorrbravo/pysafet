#include <QPainter>

#include "iconwidget.h"

IconWidget::IconWidget(QWidget *parent) :
    QLabel(parent)
{
    qDebug("... IconWidget(QWidget *parent) ... 1");


    label = new QLabel(this);
    label->setGeometry(0, 0, 91, 17);
    //label->resize(30, 15);
    //label->setFixedWidth(30);
    //label->setFixedHeight(15);
    label->setText("Listo");

    //label->setPixmap(QPixmap::QPixmap(":/yes.png"));
    //label->show();
    qDebug("... IconWidget(QWidget *parent) ... 2");

}

void IconWidget::setMessageAndImage(QString nameOfImage){
    label->setPixmap(QPixmap::QPixmap(":/"+nameOfImage));
}

void IconWidget::setTextOfLabel(QString text){
    label->setText(text);

}

QSize IconWidget::sizeHint() const
{
    return QSize(20,20);
}

void IconWidget::paintEvent(QPaintEvent * event){

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int width = qMin(this->width(), this->height());

    int outerRadius = (width-1)*0.5;
    int innerRadius = (width-1)*0.5*0.38;

    int capsuleHeight = outerRadius - innerRadius;
    int capsuleWidth  = (width > 32 ) ? capsuleHeight *.23 : capsuleHeight *.35;
    int capsuleRadius = capsuleWidth/2;

    for (int i=0; i<12; i++)
    {
        QColor color = Qt::black;
        color.setAlphaF(1.0f - (i/12.0f));
        p.setPen(Qt::NoPen);
        p.setBrush(color);
        p.save();
        p.translate(rect().center());
        p.rotate(0 - i*30.0f);
        p.drawRoundedRect(-capsuleWidth*0.5, -(innerRadius+capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
        p.restore();
    }
}



