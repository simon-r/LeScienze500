#ifndef STARTPDF_H
#define STARTPDF_H

#include <QThread>
#include <QString>
#include <QMutex>

class StartPdf : public QThread
{
public:
    StartPdf();

    void openPdf( QString p ) ;

protected:

     void run() ;

private:

    QString pdf_path ;
    QMutex mutex ;

};

#endif // STARTPDF_H
