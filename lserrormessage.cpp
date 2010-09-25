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
    res.setFileName( ":html/error.html" );
    res.open(QIODevice::ReadOnly | QIODevice::Text) ;

    QString full_message = res.readAll() ;

    res.close();
    qDebug() << full_message ;

    full_message.replace( QRegExp( "<!--nome_errore-->" ) , error_name ) ;
    full_message.replace( QRegExp( "<!--messaggio_errore-->" ) , message ) ;



    ui->ErrorMessage->setHtml( full_message );
}


