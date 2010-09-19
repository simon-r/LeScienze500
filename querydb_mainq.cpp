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


#include "querydb.h"
#include <sqlite3.h>
#include "lescienze500.h"
#include <QDebug>
#include "configls500.h"
#include <QPair>
#include <QtAlgorithms>


bool StrIntLessThan(const QString &s1, const QString &s2)
{
    return s1.toInt() < s2.toInt() ;
}

QueryResult QueryDB::execMainQuery()
{
    QString query ;

    QString logical_global = " AND " ;

    bool prevq = false ;

    if ( p_chiave )
    {
        QString logical_at = " OR " ;
        QString logical = " AND " ;

        query.append( " ( " ) ;

        if ( parole_chiave.size() > 0 )
        {
            query.append( "( " ) ;
            for ( QStringList::iterator it = parole_chiave.begin() ; it < parole_chiave.end() ; it++ )
            {
                QString first_word , word , last_word ;

                first_word.append( *it ) ; first_word.append( " %" ) ;
                word.append("%") ; word.append(*it) ; word.append("%") ;
                last_word.append("% ") ; last_word.append(*it) ;

                query.append(" ( titolo like \"") ;
                query.append( first_word ) ;
                query.append( "\" or " ) ;

                query.append("titolo like \"") ;
                query.append( word ) ;
                query.append( "\" or " ) ;

                query.append("titolo like \"") ;
                query.append( last_word ) ;
                query.append( "\" ) " ) ;

                if ( it+1 != parole_chiave.end() )
                    query.append( logical ) ;
            }
            query.append( " ) " ) ;
        }
        else
        {
            query.append( "( titolo like null )" ) ;
        }


        query.append( logical_at ) ;

        if ( parole_chiave_abstract.size() > 0 )
        {
            query.append( "( " ) ;
            for ( QStringList::iterator it = parole_chiave_abstract.begin() ; it < parole_chiave_abstract.end() ; it++ )
            {
                QString first_word , word , last_word ;

                first_word.append( *it ) ; first_word.append( " %" ) ;
                word.append("%") ; word.append(*it) ; word.append("%") ;
                last_word.append("% ") ; last_word.append(*it) ;

                query.append(" ( abstract like \"") ;
                query.append( first_word ) ;
                query.append( "\" or " ) ;

                query.append("abstract like \"") ;
                query.append( word ) ;
                query.append( "\" or " ) ;

                query.append("abstract like \"") ;
                query.append( last_word ) ;
                query.append( "\" ) " ) ;

                if ( it+1 != parole_chiave_abstract.end() )
                    query.append( logical ) ;
            }
            query.append( " )" ) ;
        }
        else
        {
             query.append( "( abstract like null )" ) ;
        }
        query.append( " )" ) ;

        prevq = true ;
    }

    //list<string> lista_rubriche = this->getRubriche() ;
    if ( rubriche && lista_rubriche.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        query.append( " ( idrubrica in ( " ) ;

        query.append( " select id from rubriche where " ) ;

        for ( QStringList::iterator it = lista_rubriche.begin() ; it < lista_rubriche.end() ; it++ )
        {
            query.append( " ( idrubrica in (  " ) ;
            query.append( " select id from rubriche where rubrica like \"" );
            query.append( *it ) ;
            query.append( "\" ) ) " ) ;
            if ( it < lista_rubriche.end() - 1 )
            {
                query.append( " OR " ) ;
            }
        }

        query.append( " ) ) " ) ;

        prevq = true ;
    }

    if ( categorie && lista_categorie.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        query.append( " ( articoli.id in ( " ) ;
        query.append( " select idarticolo from articoli_categorie where " ) ;
        for ( QStringList::iterator it = lista_categorie.begin() ; it < lista_categorie.end() ; it++ )
        {
            query.append( " ( idcategoria in (  " ) ;
            query.append( " select id from categorie where categoria like \"" );
            query.append( *it ) ;
            query.append( "\" ) ) " ) ;
            if ( it < lista_categorie.end() - 1 )
            {
                query.append( " OR " ) ;
            }
        }

        query.append( " ) ) " ) ;
        prevq = true ;
    }



    if ( autori_l && lista_autori.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        query.append( " ( articoli.id in ( " ) ;
        query.append( " select idarticolo from articoli_autori where " ) ;

        for ( QStringList::iterator it = lista_autori.begin() ; it < lista_autori.end() ; it++ )
        {
            query.append( " ( idautore in ( " ) ;
            query.append( " select id from autori where Autore like \"" ) ;
            query.append( *it ) ;
            query.append( "\" ) ) " ) ;
            if ( it < lista_autori.end() - 1 )
            {
                query.append( " OR " ) ;
            }
        }

        query.append( " ) ) " ) ;
        prevq = true ;
    }

    if ( anno && lista_anni.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        typedef QPair<int,int> yearsInterval ;
        QList<yearsInterval> years ;

        int a1 , a2 ;

        qSort( lista_anni.begin() , lista_anni.end() , StrIntLessThan ) ;

        for ( QStringList::iterator it = lista_anni.begin() ; it < lista_anni.end() ; it++ )
        {
            qDebug() << *it ;

            a1 = (*it).toInt() ;

            if ( it+1 < lista_anni.end() )
                a2 = (*(it+1)).toInt() ;
            else
                a2 = a1 ;

            if ( ( a2 - a1 ) == 1  )
            {
                int a3 = a1 ;
                int a4 ;
                QStringList::iterator it_b ;
                for ( it_b = it + 1 ; it_b < lista_anni.end() ; it_b++ )
                {
                    a4 = (*it_b).toInt() ;
                    if ( a4 - a3 == 1 )
                        a3 = a4 ;
                    else
                        break ;
                }
                yearsInterval y_int ;
                y_int.first = a1 ;
                y_int.second = a3 ;
                years.append( y_int );

                it = it_b-1 ;

                qDebug() << "Intervallo: " << a1 << " " << a3 ;
            }
            else
            {
                yearsInterval y_int ;
                y_int.first = a1 ;
                y_int.second = a1 ;
                years.append( y_int );

                qDebug() << "Intervallo: " << a1 << " " << a1 ;
            }
        }

        query.append( " ( articoli.idrivista in ( " ) ;
        query.append( " select id from riviste where " ) ;
        for ( QList<yearsInterval>::iterator it = years.begin() ; it < years.end() ; it++ )
        {
            query.append( " ( " ) ;
            query.append( " anno >= " ) ;
            query.append( QString::number( it->first ) ) ;
            query.append( " and anno <= " ) ;
            query.append( QString::number( it->second ) ) ;
            query.append( " ) " ) ;
            if ( it < years.end() - 1 )
            {
                query.append( " or " ) ;
            }
        }

        query.append( " ) ) " ) ;
        prevq = true ;
    }

    if ( testo )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }
        buildQuerySegTestoEsteso( query ) ;
        prevq = true ;
    }

    query.append( " ORDER BY idrivista" ) ;

    //query.prepend( "SELECT titolo,idRivista,id FROM articoli WHERE " ) ;
    query.prepend( "SELECT titolo,numero,articoli.id FROM articoli, riviste WHERE articoli.idrivista = riviste.id and " ) ;

    qDebug() << query  ;

    QueryResult q_result = execQuery( query ) ;
    q_result.printResult() ;

    return q_result ;
}


void QueryDB::buildQuerySegTestoEsteso( QString &query )
{

    query += " ( articoli.id in ( " ;
    query += " select idarticolo from ricercaestesa where " ;

    if ( frasi_testo_esteso.size() == 0 )
    {
        query += " ( testoesteso like null )" ;
    }
    else
    {
        QString logical_ext = " and " ;

        for ( QStringList::iterator it = frasi_testo_esteso.begin() ; it < frasi_testo_esteso.end() ; it++ )
        {
            query += " ( testoesteso like " ;
            query += "\"%" ;
            query += *it ;
            query += "%\" ) " ;

            if ( it < frasi_testo_esteso.end() - 1 )
            {
                query += logical_ext ;
            }

        }
    }
    query += " ) ) " ;
}
