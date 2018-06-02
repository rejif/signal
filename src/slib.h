#ifndef SLIB_H
#define SLIB_H

#include <QDebug>
#include <QObject>
#include <QApplication>
#include <QDesktopWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QThread>
#include <QScrollArea>

enum NFT{ DEFAULT=-1,FALSE,TRUE };

enum MVPosition{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER
};

class Slib : public QObject{
    Q_OBJECT
public:
    explicit Slib(QObject *parent = 0) :QObject(parent){}
    //WidgetHelper
    static void quitConfirm(){
        if (QMessageBox::Yes == QMessageBox::question(
            nullptr,
            "Close Confirmation",
            "Are you sure you want to exit?",
            QMessageBox::Yes|QMessageBox::No)
        ){
            qApp->quit();
        }
    }
    static void mv(QWidget* widget,MVPosition position=MVPosition::CENTER){
        QRect aa(QApplication::desktop()->availableGeometry());
        switch (position) {
            case TOP_LEFT:
                widget->move(0,0);
                break;
            case TOP_RIGHT:
                widget->move(aa.width()-widget->width(),0);
                break;
            case BOTTOM_LEFT:
                widget->move(0,aa.height()-widget->height());
                break;
            case BOTTOM_RIGHT:
                widget->move(aa.width()-widget->width(),aa.height()-widget->height());
                break;
            default:
                widget->move(aa.width()/2-widget->width()/2,aa.height()/2-widget->height()/2);
                break;
        }
    }
    static void addStyle(QWidget* widget,QString stylesheet){
        widget->setStyleSheet(widget->styleSheet().append(stylesheet));
    }
    static void showHide(QWidget* widget,NFT ntf=NFT::DEFAULT){
        if(ntf==NFT::DEFAULT){
            widget->isHidden()?widget->show():widget->hide();
        }else if(ntf==NFT::FALSE){
            widget->hide();
        }else if(ntf==NFT::TRUE){
            widget->show();
            widget->activateWindow();
        }
    }
    static void windowStaysOnTop(QWidget* widget,NFT ntf=NFT::DEFAULT){
        Qt::WindowFlags flags = widget->windowFlags();
        widget->setWindowFlags(flags |= Qt::CustomizeWindowHint);
        if(ntf==NFT::DEFAULT){
            widget->setWindowFlags(flags ^ ( Qt::WindowStaysOnTopHint));
        }else if(ntf==NFT::FALSE){
            widget->setWindowFlags(flags & (~Qt::WindowStaysOnTopHint));
        }else if(ntf==NFT::TRUE){
            widget->setWindowFlags(flags |= Qt::WindowStaysOnTopHint);
        }
        widget->hide();
        QThread::sleep(1);
        widget->show();
    }
    //Factory
    static QAction* createSlotAction(const QString actText,const QObject *receiver, const char *member, Qt::ConnectionType = Qt::AutoConnection){
        //createSlotAction(tr("SlotAction"),qApp, SLOT(quit()));
        QAction* act = new QAction(actText);
        connect(act, SIGNAL(triggered()),receiver,member);
        return act;
    }
    static QPushButton* createSlotActionButton(const QString btnText,const QObject *receiver, const char *member){
        QPushButton *slotBtn = new QPushButton(btnText);
        QAction* slotAct = createSlotAction(btnText+"Action",receiver,member);
        QObject::connect(
            slotBtn,SIGNAL(clicked(bool)),
            slotAct,SLOT(trigger())
        );
        return slotBtn;
    }
    template<typename Functor>
    static QAction* createLambdaAction(const QString actText,Functor f){
        QAction* act = new QAction(actText);
        QObject::connect(act,&QAction::triggered,f);
        return act;
    }
    template<typename Functor>
    static QPushButton* createLambdaActionButton(QString btnText,Functor f){
       QPushButton *lambdaBtn = new QPushButton(btnText);
       QAction* lambdaAct = createLambdaAction(btnText+"Action",f);
       QObject::connect(
           lambdaBtn,SIGNAL(clicked(bool)),
           lambdaAct,SLOT(trigger())
       );
       return lambdaBtn;
    }
    static QPushButton* createQAppQuitBtn(){
        QPushButton *qQuitBtn;
        qQuitBtn = new QPushButton("Quit");
        QObject::connect(qQuitBtn, &QPushButton::clicked, [=]() {
            QApplication::quit();
        });
        return qQuitBtn;
    }
    static QScrollArea* createScrollArea(QVBoxLayout *layout,bool aligncenter){
        QScrollArea* scrollarea = new QScrollArea();
        scrollarea->setWidgetResizable(true);
        scrollarea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        if(aligncenter){
            scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            layout->setAlignment(Qt::AlignCenter);
            scrollarea->setStyleSheet("background-color:rgba(34,34,34,0.8);border:none;");
            QWidget *scrollAreaWidget = new QWidget();
            scrollAreaWidget->setAutoFillBackground(false);
            scrollAreaWidget->setStyleSheet(
                "background-color:transparent;"
            );
            scrollarea->setWidget(scrollAreaWidget);
            scrollAreaWidget->setLayout(layout);
        }else{
            scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            scrollarea->setStyleSheet("background-color:transparent;border:none;");
            QWidget *scrollAreaWidget = new QWidget();
            scrollarea->setWidget(scrollAreaWidget);
            scrollAreaWidget->setLayout(layout);
        }
        return scrollarea;
    }
    static QVBoxLayout* createVBoxLayout(){
        QVBoxLayout *vl = new QVBoxLayout;
        vl->setSpacing(0);
        vl->setMargin(0);
        return vl;
    }
    static QHBoxLayout* createHBoxLayout(){
        QHBoxLayout *hl = new QHBoxLayout;
        hl->setSpacing(0);
        hl->setMargin(0);
        return hl;
    }
};

#endif // SLIB_H
