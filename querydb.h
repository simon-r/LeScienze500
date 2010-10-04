//     Copyright 2010 Simone Riva
//     mail:  simone (dot) rva (lumachina) gmail.com
//
//    This file is part of LeScienze500.
//
//    LeScienze500 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    LeScienze500 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with LeScienze500.  If not, see <http://www.gnu.org/licenses/>.



#ifndef QUERYDB_H
#define QUERYDB_H

#include <QObject>
#include <sqlite3.h>
#include <string>
#include <list>
#include <QStringList>
#include "queryresult.h"
#include <QString>

using namespace std ;
//using namespace Ui ;

class QueryDB
{ 
public:
    QueryDB();

    QueryResult getCategorie() ;
    QueryResult getAutori( QString filtro = "" ) ;
    QueryResult getAnni() ;
    QueryResult getRubriche() ;

    //bool setDBPath(string path) { return false ; } ;

    void setParoleChiave( QStringList pc ) { parole_chiave = pc ; }

    void setParoleChiaveAbstract( QStringList pc ) { parole_chiave_abstract = pc ; }

    void setFrasiTestoEsteso( QStringList te ) { frasi_testo_esteso = te ; }

    void setListaRubriche( QStringList lr )
    {
        lista_rubriche = lr ;
    }

    void setListaCategorie( QStringList lc )
    {
        lista_categorie = lc ;
    }

    void setListaAutori( QStringList la )
    {
        lista_autori = la ;
    }

    void setListaAnni( QStringList la )
    {
        lista_anni = la ;
    }

    QueryResult execMainQuery() ;

    QueryResult execQuery( QString query ) ;

    static bool execNAQuery(QString db_path , QString query ) ;

    bool p_chiave ;
    bool testo ;
    bool categorie ;
    bool autori_l ;
    bool autori_n ;
    bool anno ;
    bool rubriche ;

    QString dbPath ;

private:


   void buildQuerySegTestoEsteso( QString &query ) ;

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int col, cols;

    QStringList parole_chiave ;
    QStringList parole_chiave_abstract ;
    QStringList frasi_testo_esteso ;
    QStringList lista_rubriche ;
    QStringList lista_categorie ;
    QStringList lista_autori ;
    QStringList lista_anni ;
};

bool StrIntLessThan(const QString &s1, const QString &s2) ;


#endif // QUERYDB_H

