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

    this->dictionary.clear();

    QString query_art_id = "select Id from Articoli" ;
    configLS500 cfg ;

    QueryDB db ;
    QueryResult id_articoli ;

    db.execQuery( cfg.getDBPath() , query_art_id , id_articoli ) ;

    int e_cnt = 0 ;
    for ( QueryResult::iterator itr = id_articoli.begin() ; itr < id_articoli.end() ; itr++ )
    {

        QString id = id_articoli.getField( "Id" , itr ) ;
        qDebug() << id ;
        qDebug() << this->dictionary.size() ;

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
        this->fillDataBase() ;

        if( e_cnt++ == 100 ) return true ;
    }

    this->dictionary.clear();
    return true ;
}

bool Dictionary::addText( QString text , QString id_art )
{
    QueryDB db ;
    QueryResult anno ;
    QueryResult categorie ;

    QRegExp reg_word ;
    reg_word.setPattern( "([a-zA-Z]+)" );

    QString word ;

    configLS500 cfg ;
    QString db_path = cfg.getDictionaryPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    QString query_categoria = "select Categoria from categorie where id in ( select idcategoria from articoli_categorie where idarticolo = " ;
    query_categoria += id_art  ;
    query_categoria += " )"  ;

    db.execQuery( cfg.getDBPath() , query_categoria , categorie ) ;

    QString query_anno = "SELECT anno FROM riviste WHERE id in ( " ;
    query_anno += " select  IdRivista from articoli where Id = " ;
    query_anno += id_art ;
    query_anno += " ) " ;

    db.execQuery( cfg.getDBPath() , query_anno , anno ) ;

    int year ;
    year = anno.getField(0,0).toInt() ;
    //qDebug() << year ;

    int count = 0;
    int pos = 0;
    while ( ( pos = reg_word.indexIn(text, pos) ) != -1 )
    {
        count++;
        word = text.mid( pos , reg_word.matchedLength() ) ;

        WordInfo w_info = this->dictionary.value( word , WordInfo() ) ;

        w_info.word = word ;
        w_info.cnt++ ;

        QPair<int,int> info_year = w_info.year_cnt.value( year , QPair<int,int>(0,0) ) ;
        info_year.first = year ;
        info_year.second++ ;

        //qDebug() << word << " " << w_info.cnt << " " << year << " " << info_year.second ;

        w_info.year_cnt.insert( year , info_year ) ;

        for( QueryResult::iterator itr = categorie.begin() ; itr < categorie.end() ; itr++ )
        {
            int cat_id = categorie.getField(0,itr).toInt() ;
            QPair<int,int> info_cat = w_info.categoriaid_cnt.value( cat_id , QPair<int,int>(0,0) ) ;
            info_cat.first = cat_id ;
            info_cat.second++ ;
            w_info.categoriaid_cnt.insert( cat_id , info_cat ) ;
        }

        this->dictionary.insert( word , w_info ) ;
        pos += reg_word.matchedLength() ;
    }

    return true ;
}

bool Dictionary::fillDataBase()
{

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


