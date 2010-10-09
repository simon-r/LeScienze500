#include "bookmarkgui.h"
#include "ui_bookmarkgui.h"
#include "bookmark.h"

BookmarkGui::BookmarkGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkGui)
{
    ui->setupUi(this);
    fillCategorie() ;
}

BookmarkGui::~BookmarkGui()
{
    delete ui;
}

void BookmarkGui::fillCategorie()
{
     QList<QTreeWidgetItem *> items;
     Bookmark bk ;
     QueryResult qr ;

     bk.getMainCategorie( qr );

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
         QString name = qr.getField( "Categoria" , itr ) ;
         QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , QStringList( QString( name ) ) ) ;
         fillCategorieRec( name , item ) ;
         items.append( item ) ;
     }
     ui->treeCategorie->insertTopLevelItems(0, items);
}

void BookmarkGui::fillCategorieRec( const QString& name , QTreeWidgetItem* parent )
{
     Bookmark bk ;
     QueryResult qr ;

     bk.getCategorie( qr , name ) ;

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "Categoria" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent , QStringList( QString( name ) ) ) ;
          this->fillCategorieRec( name , item ) ;
     }
}
