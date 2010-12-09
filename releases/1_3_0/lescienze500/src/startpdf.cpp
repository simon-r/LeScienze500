#include "startpdf.h"
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include "configls500.h"

StartPdf::StartPdf()
{
}

void StartPdf::run()
{
    QString pdf_appl , command ;
    bool process_strated ;

    QProcess process_pdf ;

    configLS500 cfg ;
    pdf_appl = cfg.getPDFAppl() ;

    command.append( pdf_path ) ;
    command.append( " " ) ;
    command.append( "\"" ) ;

    command.append( pdf_path ) ;
    command.append( "\"" ) ;

    //qDebug() << command ;

    if ( pdf_appl != "desktop" )
        process_strated = process_pdf.startDetached( command );
    else
    {
        QUrl url ;
        url.setScheme( "file" );
        url.setPath( pdf_path );
        process_strated = QDesktopServices::openUrl( url ) ;
    }
}

void StartPdf::openPdf( QString p )
{
    mutex.lock();
    this->pdf_path = p ;

    this->start();

    mutex.unlock();
}





