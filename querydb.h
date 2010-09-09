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

    list<string> getCategorie() ;
    list<string> getAutori( string filtro = "" ) ;
    list<int> getAnni() ;
    list<string> getRubriche() ;

    //bool setDBPath(string path) { return false ; } ;

    void setParoleChiave( QStringList pc )
    {
        parole_chiave = pc ;
    }

    void setParoleChiaveAbstract( QStringList pc )
    {
        parole_chiave_abstract = pc ;
    }

    void setListaRubriche( QStringList lr )
    {
        lista_rubriche = lr ;
    }

    void setListaCategorie( QStringList lr )
    {
        lista_categorie = lr ;
    }

    QueryResult execMainQuery() ;

    QueryResult execQuery( QString query ) ;

    bool p_chiave ;
    bool testo ;
    bool categorie ;
    bool autori_l ;
    bool autori_n ;
    bool anno ;
    bool rubriche ;

    QString dbPath ;

private:

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int col, cols;

    QStringList parole_chiave ;
    QStringList parole_chiave_abstract ;
    QStringList lista_rubriche ;
    QStringList lista_categorie ;
};

#endif // QUERYDB_H

