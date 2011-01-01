#include "dictionary.h"

#include "configls500.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QString>
#include <sqlite3.h>
#include <QTime>
#include <QRegExp>
#include "querydb.h"

Dictionary::Dictionary()
{
}


bool Dictionary::buildDictionary()
{
    QString query_art_id = "select Id from Articoli" ;

    configLS500 cfg ;

    QueryDB db ;
    QueryResult id_articoli ;

    db.execQuery( cfg.getDBPath() , query_art_id , id_articoli ) ;

    for ( QueryResult::iterator itr = id_articoli.begin() ; itr < id_articoli.end() ; itr++ )
    {
        QString id = id_articoli.getField( "Id" , itr ) ;

        QString query_testo_esteso = "select TestoEsteso from RicercaEstesa where IdArticolo = " ;
        query_testo_esteso += id ;

        QString query_titolo =  "select Titolo , Abstract from articoli where Id = " ;
        query_titolo += id ;

        QueryResult qr_testo_esteso ;
        db.execQuery( cfg.getDBPath() , query_testo_esteso , qr_testo_esteso );

        QueryResult qr_titolo ;
        db.execQuery( cfg.getDBPath() , query_titolo , qr_titolo );

        QString full_text = " " ;
        if ( !qr_testo_esteso.empty() )
        {
            full_text = qr_testo_esteso.getField( "TestoEsteso" , qr_testo_esteso.begin() ) ;
            full_text += " " ;
        }

        if ( !qr_titolo.empty() )
        {
            full_text +=  qr_titolo.getField( "Titolo" , qr_titolo.begin() ) ;
            full_text += " " ;
            full_text +=  qr_titolo.getField( "Abstract" , qr_titolo.begin() ) ;
            full_text += " " ;
        }

    }

    return true ;
}







