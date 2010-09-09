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

class QueryResult {
public:

    QueryResult()
    {
    }

    void appendColumnName( QString name ) ;
    bool appendResultRow( QStringList res ) ;

    QList<QStringList> getResultTable()
    {
        return q_result ;
    }

    QList<QString> getColumnsNames()
    {
        return columns_names ;
    }

    void printResult() ;

    void clear()
    {
        columns_names.clear();
        q_result.clear();
    }

   QList<QString> columns_names ;
   QList<QStringList> q_result ;

   private:
};

#endif // QUERYRESULT_H
