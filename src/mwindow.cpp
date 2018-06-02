#include "mwindow.h"
MWindow::MWindow(QWidget *parent):QMainWindow(parent){
    setWindowTitle(tr("Signal"));

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setAutoFillBackground(false);

    setWindowIcon(QIcon(":/signal.png"));
    setParent(0);
    setStyleSheet("QMainWindow{background:transparent;}");

//        setMaximumWidth(800);
//        setMaximumHeight(600);
//        resize(this->maximumWidth(),this->maximumHeight());

    config->load();
    Glass *cw = new Glass();
    setCentralWidget(cw);
    QVBoxLayout *vl = Slib::createVBoxLayout();
    cw->setLayout(vl);

    QHBoxLayout *hl = Slib::createHBoxLayout();
    QLabel *title = new QLabel(getUsername()+"@Signal");
    title->setStyleSheet(
                "QLabel{"
                    "padding:1px 5px;"
                    "color:white;"
                    "background-color:rgba(34,34,34,0.8);"
                "}"
                "QLabel:hover:!pressed{"
                    "background-color:#5fe35f;"
                "}"
            );
    hl->addWidget(title);
    hl->addWidget(Slib::createSlotActionButton("ShowHide",this,SLOT(mwShowHide())));
    hl->addWidget(Slib::createQAppQuitBtn());
    vl->addLayout(hl);
    vl->addWidget(Slib::createLambdaActionButton("ConfigReLoad",[=](){
        config->load();
        QMessageBox::information(this,"Information","token:"+config->token);
    }));
    vl->addWidget(Slib::createLambdaActionButton("ConfigEdit",[=](){if(config->check()){QProcess::execute("C:/Windows/System32/cmd.exe /C start "+config->path);}}));
    vl->addWidget(Slib::createLambdaActionButton("ConfigSave",[=](){config->save();}));
    vl->addWidget(
        Slib::createLambdaActionButton("EPOCID",[=](){
            QApplication::clipboard()->setText(QString::number(getEpoc()));
        })
    );
    vl->addStretch();

}
MWindow::~MWindow(){}

