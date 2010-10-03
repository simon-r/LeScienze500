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
    typedef QStringList::Iterator col_iterator ;

    QueryResult()
    {
    }

    void appendColumnName( QString name , int col ) ;
    bool appendResultRow( QStringList res ) ;

    QList<QStringList> getResultTable()
    {
        return q_result ;
    }

    int size()
    {
        return q_result.size() ;
    }

    QStringList getColumnsNames()
    {
        return columns_names ;
    }

    QString getColumnName( int i )
    {
        return columns_names[i] ;
    }

    int getColumnsCnt()
    {
        return columns_names.size() ;
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

    QString getField( QueryResult::col_iterator col , QueryResult::iterator row )
    {
        QString col_name = *col ;
        return getField( col_name , row ) ;
    }

    QString getField( int col_index , QueryResult::iterator row )
    {
        return row->value( col_index ) ;
    }

    QString getFiled( int col_index , int row_index )
    {
        return this->q_result[row_index][col_index] ;
    }

    QString getFirstColumnName()
    {
        if ( !columns_names.empty() )
            return columns_names.first() ;
        else
            return QString( "" ) ;
    }

    QStringList getColumnNames()
    {
        return columns_names ;
    }

    int sizeCol() { return columns_names.size() ; }
    int sizeRow() { return q_result.size() ; }

    bool empty() { return q_result.empty() ; }

    QueryResult::iterator begin() { return q_result.begin() ; }
    QueryResult::iterator end() { return q_result.end() ; }

    QueryResult::col_iterator colBegin() { return columns_names.begin() ; }
    QueryResult::col_iterator colEnd() { return columns_names.end() ; }

    QStringList columns_names ;
    QList<QStringList> q_result ;

private:



    QHash<QString,int> col_name_index ;
};

#endif // QUERYRESULT_H
