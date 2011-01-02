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


bool Dictionary::exists()
{
    configLS500 cfg ;

    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    QFile file ;
    file.setFileName( db_path );

    return file.exists() ;
}

 bool Dictionary::initDictionary()
 {
     configLS500 cfg ;
     bool ret ;

     QString db_path = cfg.getDictionaryPath() ;
     db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

     if ( !exists() )
     {
         QFile res ;
         res.setFileName(":/sql/sql/dizionario.sql" );
         res.open(QIODevice::ReadOnly) ;
         QString full_sql = QString::fromUtf8( res.readAll() ) ;
         qDebug() << full_sql ;
         res.close();

         ret = QueryDB::execNAQuery( db_path , full_sql ) ;
     }
     else
         ret = false ;

     return ret ;
 }

bool Dictionary::buildDictionary()
{
    this->initDictionary() ;
    this->addYears() ;

    QString query_art_id = "select Id from Articoli" ;

    configLS500 cfg ;

    QueryDB db ;
    QueryResult id_articoli ;

    db.execQuery( cfg.getDBPath() , query_art_id , id_articoli ) ;

    for ( QueryResult::iterator itr = id_articoli.begin() ; itr < id_articoli.end() ; itr++ )
    {

        QString id = id_articoli.getField( "Id" , itr ) ;
        qDebug() << id ;

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

        this->addText( full_text , id ) ;
        return true ;
    }

    return true ;
}

bool Dictionary::addText( QString text , QString id_art )
{
    QRegExp reg_word ;
    reg_word.setPattern( "([a-zA-Z]+)" );

    QString word ;
    QStringList article_dict ;

    int count = 0;
    int pos = 0;
    while ( ( pos = reg_word.indexIn(text, pos) ) != -1 )
    {
        count++;
        word = text.mid( pos , reg_word.matchedLength() ) ;
        article_dict.append( word ) ;
        pos += reg_word.matchedLength() ;
    }


}


bool Dictionary::addYears()
{
    QueryDB db ;
    QueryResult anni = db.getAnni() ;

    configLS500 cfg ;
    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    bool ret = true ;
    QString insert ;
    for ( QueryResult::iterator itr = anni.begin() ; itr < anni.end() ; itr++ )
    {
        insert += " insert into Anni ( Anno ) values (  " ;
        insert += anni.getField( "Anno" , itr ) ;
        insert += " ) ; " ;


    }

    qDebug() <<  db_path << insert ;
    ret = ret && QueryDB::execNAQuery( db_path , insert ) ;
    return ret ;
}

bool Dictionary::addCategorie()
{

}

bool Dictionary::addIdArticoli()
{

}


