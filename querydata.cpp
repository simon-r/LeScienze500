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

#include "querydata.h"

QueryData::QueryData()
{
    g_logical = QueryData::QEUERY_AND ;
}

void QueryData::setParoleChiaveTitolo( const QString &pct )
{
    parole_chiave_titolo = pct ;
}

void QueryData::setParoleChiaveAbstract( const QString &pca )
{
    parole_chiave_abstract = pca ;
}

void QueryData::setParoleChiaveTesto( const QString &pct )
{
    parole_chiave_testo = pct ;
}

void QueryData::setCategorie( const QStringList &cat ) {
    categorie = cat ;
}

void QueryData::setRubriche( const QStringList &ru ) {
    rubriche = ru ;
}

void QueryData::setAnni( const QStringList &a ) {
    anni = a  ;
}

void QueryData::setAutori( const QStringList aut ) {
    autori = aut ;
}



QString QueryData::getParoleChiaveTitolo()
{
    return parole_chiave_titolo ;
}

QString QueryData::getParoleChiaveAbstract()
{
    return parole_chiave_abstract ;
}

QString QueryData::getParoleChiaveTesto()
{
    return parole_chiave_testo ;
}

QStringList QueryData::getCategorie()
{
    return categorie ;
}

QStringList QueryData::getRubriche()
{
    return rubriche ;
}

QStringList QueryData::getAnni()
{
    return anni ;
}

QStringList QueryData::getAutori()
{
    return autori ;
}
