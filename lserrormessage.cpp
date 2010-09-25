#include "lserrormessage.h"
#include "ui_lserrormessage.h"
#include <QFile>
#include <QDebug>

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

    QString full_message = res.readAll() ;

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

    QString message = res.readAll() ;

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

    QString message = res.readAll() ;

    res.close();

    message.replace( QRegExp("<!--pdf_file-->") , file_name ) ;

    setHtmlMessage( "Articolo non trovato" , message ) ;

    this->setModal( true );
    this->setFocus();
    this->show();
}

void LSErrorMessage::showReaderNotStarted()
{

}
