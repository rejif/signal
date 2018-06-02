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

    QHBoxLayout *hlheader = Slib::createHBoxLayout();
    hlheader->addWidget(Slib::createLabel(getUsername()+"@Signal",
      "QLabel{"
          "padding:1px 5px;"
          "color:white;"
          "background-color:rgba(34,34,34,0.8);"
      "}"
      "QLabel:hover:!pressed{"
          "background-color:#5fe35f;"
      "}"
    ));
    hlheader->addWidget(Slib::createSlotActionButton("ShowHide",this,SLOT(mwShowHide())));
    hlheader->addWidget(Slib::createQAppQuitBtn());
    vl->addLayout(hlheader);

    vl->addSpacing(23);

    vl->addWidget(
        Slib::createLambdaActionButton("ApplicationUpdate",[=](){
            QProcess::execute("C:/Windows/System32/cmd.exe /C start https://ci.appveyor.com/project/onoie/signal/build/artifacts/");
        })
    );

    QHBoxLayout *hlconfig = Slib::createHBoxLayout();
    hlconfig->addWidget(Slib::createLabel("Config","QLabel{padding:1px 5px;color:white;background-color:rgba(34,34,34,0.8);}"));
    hlconfig->addWidget(Slib::createLambdaActionButton("ReLoad",[=](){
        config->load();
        QMessageBox::information(this,"Information","token:"+config->token);
    }));
    hlconfig->addWidget(Slib::createLambdaActionButton("ConfigEdit",[=](){if(config->check()){QProcess::execute("C:/Windows/System32/cmd.exe /C start "+config->path);}}));
    hlconfig->addWidget(Slib::createLambdaActionButton("ConfigSave",[=](){config->save();}));
    vl->addLayout(hlconfig);

    vl->addWidget(
        Slib::createLambdaActionButton("EPOCID",[=](){
            QApplication::clipboard()->setText(QString::number(getEpoc()));
        })
    );
    vl->addStretch();

}
MWindow::~MWindow(){}

