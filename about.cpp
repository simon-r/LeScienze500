#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>
#include <QFile>
#include <QRegExp>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    connect( ui->Credits , SIGNAL(linkActivated(QString)), this , SLOT(on_linkActivated(QString))) ;
    connect( ui->AboutText , SIGNAL(anchorClicked(QUrl)) , this ,SLOT(on_linkMessageClicked(QUrl)) ) ;


    QFile res ;
    res.setFileName( ":/html/html/copyright_message.html" );
    res.open(QIODevice::ReadOnly) ;
    QString message = QString::fromLocal8Bit ( res.readAll() ) ;
    res.close();

    QString autore ;
    QString ema ;
    ema += "simone" ; ema += "." ;

    autore += "Simone" ;
    autore += " " ;
    ema.append("rva") ;
    autore += "Riva" ;
    ema += "@" ;
    ema.append( "gmail" ) ;
    ema += "." ;
    ema += "com" ;

    message.replace( QRegExp("<!--autore-->") , autore ) ;
    message.replace( QRegExp("<!--email-->") , ema ) ;

    ui->AboutText->setHtml( message ) ;
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

void About::on_linkMessageClicked( const QUrl& url )
{
    QDesktopServices::openUrl ( url ) ;
}
