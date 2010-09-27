#ifndef BROWSERCOPERTINEBASE_H
#define BROWSERCOPERTINEBASE_H

#include "querydata.h"
#include <QString>
#include <QStringList>
#include "querydb.h"


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

protected:

    bool fillListaAnni() ;

    virtual void appendAnnoGUI( QString anno ) = 0 ;

    void clear() ;


};

#endif // BROWSERCOPERTINEBASE_H
