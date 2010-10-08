#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    connect( ui->Credits , SIGNAL(linkActivated(QString)), this , SLOT(on_linkActivated(QString))) ;
}

About::~About()
{
    delete ui;
}

void About::on_linkActivated( const QString& link )
{
    QUrl url( link ) ;
    QDesktopServices::openUrl ( url ) ;
}
