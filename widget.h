#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>

class Widget : public QWidget{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr) : QWidget(parent){}
protected:
    void closeEvent(QCloseEvent *event) override{
        QMessageBox::StandardButton reply=QMessageBox::question(this,"Confirm","Close?",QMessageBox::Ok|QMessageBox::Cancel);
         if (reply == QMessageBox::Ok) {
             event->accept();
         }else{
             event->ignore();
         }
    }
};

#endif // WIDGET_H
