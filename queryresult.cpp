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


#include "queryresult.h"
#include<qstring.h>
#include<qstringlist.h>
#include<qlist.h>
#include<qdebug.h>


void QueryResult::appendColumnName( QString name )
{
   this->columns_names.append(name);
}

bool QueryResult::appendResultRow( QStringList res )
{

    if ( this->columns_names.size() != res.size() )
    {
        return false ;
    }

    this->q_result.append( res ) ;

    return true;
}

void QueryResult::printResult()
{
    for( QList<QStringList>::iterator it = q_result.begin() ; it < q_result.end() ; it++ )
    {
        int i = 0 ;
        for( QStringList::iterator it_c = it->begin() ; it_c < it->end() ; it_c++ )
        {
            qDebug() << this->columns_names[i] << ": " << *it_c ;
            i++ ;
        }
    }
}
