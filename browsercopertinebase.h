#ifndef BROWSERCOPERTINEBASE_H
#define BROWSERCOPERTINEBASE_H

#include "querydata.h"
#include <QString>
#include <QStringList>

class BrowserCopertineBase
{
public:
    BrowserCopertineBase();

    void openBrowser() ;
    void openBrowser( QueryData &data ) ;
    void close() ;

    bool showAnno( const QString &anno ) ;
    bool showMeseAnno( const QString &mese , const QString &anno ) ;
    bool showRivista( const QString &numero ) ;

private:

    void clear() ;


};

#endif // BROWSERCOPERTINEBASE_H
