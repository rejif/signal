#ifndef GLASS_H
#define GLASS_H

#include <QWidget>
#include <QApplication>
#include <QPainter>

class Glass : public QWidget{
    Q_OBJECT
public:
    explicit Glass(QWidget* parent=Q_NULLPTR,Qt::WindowFlags f=Qt::WindowFlags()):QWidget(parent,f){
        setAttribute(Qt::WA_TranslucentBackground,true);
        setWindowFlags(Qt::FramelessWindowHint);
        setWindowTitle("GlassWidget");
        setStyleSheet("QGraphicsScene{border:none;}");
    }
    virtual ~Glass(){}
    QColor getColor(){ return color; }
    void setColor(QColor color){ this->color=color; }
protected:
    QColor color= QColor(34,34,34,128);
    void paintEvent(QPaintEvent *event){
        QPainter(this).fillRect(rect(),color);
        QWidget::paintEvent(event);
    }
};

#endif // GLASS_H
