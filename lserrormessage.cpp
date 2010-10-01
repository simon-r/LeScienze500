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

#include "lserrormessage.h"
#include "ui_lserrormessage.h"
#include <QFile>
#include <QDebug>
#include "configls500.h"

LSErrorMessage::LSErrorMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LSErrorMessage)
{
    ui->setupUi(this);
}

LSErrorMessage::~LSErrorMessage()
{
    delete ui;
}

void LSErrorMessage::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void LSErrorMessage::setHtmlMessage( QString error_name , QString message )
{
    QFile res ;
    res.setFileName( ":/html/html/error.html" );
    res.open(QIODevice::ReadOnly) ;

    QString full_message = QString::fromLocal8Bit ( res.readAll() ) ;

    res.close();
//    qDebug() << full_message ;

    full_message.replace( QRegExp( "<!--nome_errore-->" ) , error_name ) ;
    full_message.replace( QRegExp( "<!--messaggio_errore-->" ) , message ) ;

    ui->ErrorMessage->setHtml( full_message );
}

void LSErrorMessage::showDBError()
{
    QFile res ;

    res.setFileName( ":/html/html/db_error_msg.html" );
    res.open(QIODevice::ReadOnly) ;

    QString message = QString::fromLocal8Bit ( res.readAll() ) ;

    res.close();

    setHtmlMessage( "Errore di connessione al database" , message ) ;

    this->setModal( true );
    this->setFocus();
    this->show();
}

void LSErrorMessage::showArticleNotFound( QString file_name )
{
    QFile res ;

    res.setFileName( ":/html/html/article_not_found.html" );
    res.open(QIODevice::ReadOnly) ;

    QString message = QString::fromLocal8Bit ( res.readAll() ) ;

    res.close();
    
    message.replace( QRegExp("<!--file_name-->") , file_name ) ;

    setHtmlMessage( "Articolo non trovato" , message ) ;

    this->setModal( true );
    this->setFocus();
    this->show();
}

void LSErrorMessage::showReaderNotStarted()
{
    QFile res ;

    res.setFileName( ":/html/html/reader_not_found.html" );
    res.open(QIODevice::ReadOnly) ;

    QString message = QString::fromLocal8Bit ( res.readAll() ) ;

    res.close();

    configLS500 cfg ;

    message.replace( QRegExp("<!--reader-->") , cfg.getPDFAppl() ) ;

    setHtmlMessage( "Impossibile avviare il lettore di documenti PDF" , message ) ;

    this->setModal( true );
    this->setFocus();
    this->show();
}

void LSErrorMessage::showCopertineNotFoundError()
{
    QFile res ;

    res.setFileName( ":/html/html/copertine_not_found.html" );
    res.open(QIODevice::ReadOnly) ;

    QString message = QString::fromLocal8Bit ( res.readAll() ) ;

    res.close();

    configLS500 cfg ;

    //message.replace( QRegExp("<!--reader-->") , cfg.getPDFAppl() ) ;

    setHtmlMessage( "Impossibile trovare la directory delle copertine" , message ) ;

    this->setModal( true );
    this->setFocus();
    this->show();
}
