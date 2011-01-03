
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


QString Dictionary::convertWildcards( QString word )
{
    word.replace( "*" , "%" ) ;
    word.replace( "." , "_" ) ;

    return word ;
}

bool Dictionary::getYearsFromWord( QueryResult& query_r , QString word )
{
    word = convertWildcards( word ) ;

    QString query = "select cnt , Anni.anno from Word_Anno , Anni where Anni.Anno = Word_Anno.Anno and idword in "
                    "( select id from words where word like \"%1\" ) and Word_Anno.anno in "
                    "( select anno from anni where anno in ( select anno from Word_Anno where idword in "
                    "( select id from words where word like \"%1\" ) ) ) " ;

    query = query.arg(word) ;

    query_r.clear();
    this->execQuery( query , query_r ) ;

    return true ;
}


bool Dictionary::getCategorieFromWord( QueryResult& query_r , QString word )
{
    word = convertWildcards( word ) ;

    QString query = "select cnt , categoria from Word_Categoria , categorie where categorie.id = Word_Categoria.idcategoria and idword in "
                    "( select id from words where word like \"%1\" ) and idcategoria in "
                    "( select id from categorie where id in "
                    "( select idcategoria from Word_Categoria where idword in ( select id from words where word like \"%1\" ) ) ) order by cnt ; " ;

    query = query.arg(word) ;

    query_r.clear();
    this->execQuery( query , query_r ) ;
    return true ;
}

bool Dictionary::getWord( QueryResult& query_r , QString word , int limit )
{
    word = convertWildcards( word ) ;

    QString query = " select * from words where word like \"%1\" order by cnt desc limit %2  ; " ;

    query = query.arg(word).arg(limit) ;

    query_r.clear();
    this->execQuery( query , query_r ) ;
    return true ;
}

void Dictionary::execQuery( QString& query , QueryResult& qr )
{
    configLS500 cfg ;
    QString db_path = cfg.getDictionaryPath() ;

    QueryDB db ;
    db.execQuery( db_path , query , qr ) ;
}
