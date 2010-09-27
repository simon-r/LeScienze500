#include "browsercopertinebase.h"


BrowserCopertineBase::BrowserCopertineBase()
{
}

bool BrowserCopertineBase::fillListaAnni()
{
    QueryDB db ;
    QueryResult lista = db.getAnni() ;

    if ( lista.empty() )
        return false ;

    QString col_name = lista.getFirstColumnName() ;

    QueryResult::iterator it;

    for( it = lista.begin() ; it < lista.end() ; it++ )
    {
        this->appendAnnoGUI( lista.getField( col_name , it ) );
    }
    return true ;
}
