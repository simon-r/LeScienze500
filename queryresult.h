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


#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include<qstring.h>
#include<qlist.h>
#include<qstringlist.h>
#include<qhash.h>

class QueryResult {
public:

    typedef QList<QStringList>::iterator iterator ;

    QueryResult()
    {
    }

    void appendColumnName( QString name , int col ) ;
    bool appendResultRow( QStringList res ) ;

    QList<QStringList> getResultTable()
    {
        return q_result ;
    }

    QStringList getColumnsNames()
    {
        return columns_names ;
    }

    void printResult() ;

    void clear()
    {
        columns_names.clear();
        q_result.clear();
    }

    QString getField( QString col_name , QueryResult::iterator row )
    {
        int index = col_name_index.value( col_name ) ;
        return row->value( index ) ;
    }

    QString getFirstColumnName()
    {
        if ( !columns_names.empty() )
            return columns_names.first() ;
        else
            return QString( "" ) ;
    }

    QueryResult::iterator begin() { return q_result.begin() ; }
    QueryResult::iterator end() { return q_result.end() ; }

   QStringList columns_names ;
   QList<QStringList> q_result ;

   private:

   QHash<QString,int> col_name_index ;
};

#endif // QUERYRESULT_H
