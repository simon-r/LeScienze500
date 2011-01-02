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
#include <QHash>

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
    this->addCategorie() ;
    this->addIdArticoli() ;

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
    QueryDB db ;

    QHash<QString,int> tmp_dict ;

    QString word ;
    QStringList article_dict ;

    configLS500 cfg ;
    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    int count = 0;
    int pos = 0;
    while ( ( pos = reg_word.indexIn(text, pos) ) != -1 )
    {
        count++;
        word = text.mid( pos , reg_word.matchedLength() ) ;
        article_dict.append( word ) ;
        pos += reg_word.matchedLength() ;
    }

    QString query_categoria = "select categoria from categorie where id in ( select idcategoria from articoli_categorie where idarticolo = " ;
    query_categoria += id_art  ;
    query_categoria += " )"  ;

    QString query_anno = "SELECT anno FROM riviste WHERE id in ( " ;
    query_anno += " select  IdRivista from articoli where Id = " ;
    query_anno += id_art ;
    query_anno += " ) " ;

    QString query_word ;
    QString global_insert = "  begin ; " ;
    for ( QStringList::iterator itr = article_dict.begin() ; itr < article_dict.end() ; itr++ )
    {
        QString inser_word = "" ;
        if( word.getField(0,0).toInt() == 0 )
        {
            inser_word =  " insert into Words ( Word ) values (  \"" ;
            inser_word += *itr ;
            inser_word += "\" ) ; " ;
            //qDebug() << inser_word ;
        }
        else
        {

        }

        global_insert += inser_word ;
    }

    global_insert += " commit ; " ;
    return QueryDB::execNAQuery( db_path , global_insert ) ;

}


bool Dictionary::addYears()
{
    QueryDB db ;
    QueryResult anni = db.getAnni() ;

    configLS500 cfg ;
    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    bool ret = true ;
    QString insert = " begin ; ";
    for ( QueryResult::iterator itr = anni.begin() ; itr < anni.end() ; itr++ )
    {
        insert += " insert into Anni ( Anno ) values (  " ;
        insert += anni.getField( "Anno" , itr ) ;
        insert += " ) ; " ;
    }
    insert += " commit ; " ;

    //qDebug() <<  db_path << insert ;
    ret = ret && QueryDB::execNAQuery( db_path , insert ) ;
    return ret ;
}

bool Dictionary::addCategorie()
{
    QueryDB db ;
    QueryResult categorie = db.getCategorie() ;

    configLS500 cfg ;
    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    bool ret = true ;
    QString insert = " begin ; ";
    for ( QueryResult::iterator itr = categorie.begin() ; itr < categorie.end() ; itr++ )
    {
        insert += " insert into Categorie ( Categoria ) values ( \"" ;
        insert += categorie.getField( "Categoria" , itr ) ;
        insert += "\" ) ; " ;
    }
    insert += " commit ; " ;

    //qDebug() <<  db_path << insert ;
    ret = ret && QueryDB::execNAQuery( db_path , insert ) ;
    return ret ;
}

bool Dictionary::addIdArticoli()
{
    QString query_art_id = "select Id from Articoli order by Id " ;
    configLS500 cfg ;

    QueryDB db ;
    QueryResult id_articoli ;

    db.execQuery( cfg.getDBPath() , query_art_id , id_articoli ) ;

    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    bool ret = true ;
    QString insert = " begin ; ";
    for ( QueryResult::iterator itr = id_articoli.begin() ; itr < id_articoli.end() ; itr++ )
    {
        insert += " insert into Articoli ( IdArticolo ) values ( " ;
        insert += id_articoli.getField( "Id" , itr ) ;
        insert += " ) ; " ;
    }
    insert += " commit ; " ;

    //qDebug() <<  db_path << insert ;
    ret = ret && QueryDB::execNAQuery( db_path , insert ) ;
    return ret ;
}


