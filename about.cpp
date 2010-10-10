//     Copyright 2010 Simone Riva
//     mail:  simone (dot) rva (lumachina) gmail.com
//
//    This file is part of LeScienze500.
//
//    LeScienze500 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    LeScienze500 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with LeScienze500.  If not, see <http://www.gnu.org/licenses/>.

#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>
#include <QFile>
#include <QRegExp>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{

    QString autore ;
    QString ema ;

    QString al ;
    QString alom ;
    ui->setupUi(this);

    al += "Jacopo" ;
    ema.append( "simone" ) ; ema += "." ;
    alom.append("jacopo") ;
    autore += "Simone" ;
    alom += "fois" ;
    autore += " " ;
    al += " " ; al.append("Fois") ;

    connect( ui->Credits , SIGNAL(linkActivated(QString)), this , SLOT(on_linkActivated(QString))) ;
    connect( ui->AboutText , SIGNAL(anchorClicked(QUrl)) , this ,SLOT(on_linkMessageClicked(QUrl)) ) ;


    QFile res ;
    res.setFileName( ":/html/html/copyright_message.html" );
    res.open(QIODevice::ReadOnly) ;
    QString message = QString::fromLocal8Bit ( res.readAll() ) ;
    res.close();

    alom.append("@") ;
    ema.append("rva") ;
    autore += "Riva" ;
    alom += "gmail" ;
    ema += "@" ;
    alom.append(".") ;
    ema.append( "gmail" ) ;
    ema += "." ;
    ema += "com" ;
    alom.append("com") ;

    message.replace( QRegExp("<!--autore-->") , autore ) ;
    message.replace( QRegExp("<!--email-->") , ema ) ;

    message.replace( QRegExp("<!--autore_logo-->") , al ) ;
    message.replace( QRegExp("<!--email_logo-->") , alom ) ;

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
