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
