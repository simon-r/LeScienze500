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

     bk.getRootCategorie( qr );

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
         QString name = qr.getField( "Categoria" , itr ) ;
         QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 ) ;
         setFolderItemDecorations( item , name ) ;
         fillCategorieRec( name , item ) ;
         items.append( item ) ;
     }

     bk.getFavoritesByParent( qr , "" ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdArticolo" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 ) ;
          this->setArticleItemDecorations( item , name ) ;
          items.append( item );
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
          QTreeWidgetItem* item = new QTreeWidgetItem( parent ) ;
          this->setFolderItemDecorations( item , name ) ;
          this->fillCategorieRec( name , item ) ;
     }

     bk.getFavoritesByParent( qr , name ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdArticolo" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent ) ;
          this->setArticleItemDecorations( item , name ) ;
     }

}

void BookmarkGui::setFolderItemDecorations( QTreeWidgetItem* item , const QString& name )
{
    QIcon folder(":/icons/crystal/folder.png") ;
    item->setIcon( 0 , folder);
    item->setText( 0 , name );
}

void BookmarkGui::setArticleItemDecorations( QTreeWidgetItem* item , const QString& name )
{
    QIcon doc(":/icons/crystal/doc-icon.png") ;
    item->setIcon( 0 , doc );
    item->setText( 0 , name );
}
