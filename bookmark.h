#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QStringList>
#include <QList>
#include <queryresult.h>

class Bookmark
{
public:
    Bookmark();

    bool buildDataBase() ;

    QueryResult getFavorites() ;
    QueryResult getCategorie() ;
    QueryResult getStati() ;
    QueryResult getValutazioni() ;

    QueryResult getFavoritesByStato( QString IdStato ) ;
    QueryResult getFavoritesByCategoria( QString IdCategoria ) ;

    QueryResult getCommento( QString IdFavorito ) ;
    QueryResult getValutazione( QString IdFavorito ) ;
    QueryResult getStato( QString IdFavorito ) ;
    QueryResult getCategoria( QString IdFavorito ) ;

    QueryResult getSottoCategorie( QString IdCategoria ) ;
    QueryResult getMainCategoria( QString IdCategoria ) ;

private:

};

#endif // BOOKMARK_H
