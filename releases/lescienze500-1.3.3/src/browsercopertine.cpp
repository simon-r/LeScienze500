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

#include "browsercopertine.h"
#include "ui_browsercopertine.h"
#include<QFile>
#include<QDebug>


BrowserCopertine::BrowserCopertine(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::BrowserCopertine)
{
    ui->setupUi(this);

    ui->MostraRiviste->setOpenLinks( false );
    ui->MostraNumeroRivista->setOpenLinks( false );

    connect( ui->ListaAnni , SIGNAL( itemClicked(QListWidgetItem*)  ) , this , SLOT( on_itemListaAnniSelected(QListWidgetItem*) ) ) ;
    connect( ui->MostraRiviste , SIGNAL( anchorClicked ( const QUrl& ) ) , this , SLOT( on_rivistaClicked( const QUrl& ) ) ) ;
    connect( ui->MostraNumeroRivista , SIGNAL( anchorClicked ( const QUrl& ) ) , this , SLOT( on_indiceArticoliClicked( const QUrl& ) ) ) ;
    connect( ui->SelezioneNumero , SIGNAL(valueChanged (const QString&) ) ,  this , SLOT(  on_spinNuovoNumero(const QString&) ) ) ;
}

BrowserCopertine::~BrowserCopertine()
{
    delete ui ;
}



void BrowserCopertine::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void BrowserCopertine::appendAnnoGUI( QString anno )
{
    ui->ListaAnni->addItem( anno );
}

void BrowserCopertine::clearListaAnni()
{
    ui->ListaAnni->clear();
}

void BrowserCopertine::openBrowser()
{
    this->setWindowTitle( "Naviga nelle riviste" );
    this->fillListaAnni() ;

    this->setModal(true);
    this->setFocus();
    this->show();

   this->blankPage( BrowserCopertine::BlankAnno | BrowserCopertine::BlanckRivista );
}

void BrowserCopertine::blankPage( int bl )
{
    QFile res ;

    if ( bl & BrowserCopertine::BlankAnno )
    {
        QString blank ;

        res.setFileName( ":/html/html/blank_lista_copertine.html" );
        res.open(QIODevice::ReadOnly) ;
        blank = QString::fromUtf8(  res.readAll() ) ;
        res.close();

        ui->MostraRiviste->setHtml( blank ) ;
    }


    if ( bl & BrowserCopertine::BlanckRivista )
    {
        QString blank ;

        res.setFileName( ":/html/html/blank_rivista.html" );
        res.open(QIODevice::ReadOnly) ;
        blank = QString::fromUtf8(  res.readAll() ) ;
        res.close();

        ui->MostraNumeroRivista->setHtml( blank ) ;
    }
}

void BrowserCopertine::openListaRiviste( const QString anno )
{
    QFile res ;
    res.setFileName( ":/html/html/testata_lista_copertine.html" );
    res.open(QIODevice::ReadOnly) ;

    pagina_anno.clear();
    this->anno_c = anno ;

    pagina_anno = QString::fromUtf8( res.readAll() ) ;
    pagina_anno.replace( QRegExp("<!--anno-->") , anno ) ;

   // qDebug() << pagina_anno ;
   res.close();
}

void BrowserCopertine::appendRivista( const QString copertina , const QString mese )
{
    QFile res ;
    res.setFileName( ":/html/html/corpo_lista_copertine.html" );

    res.open(QIODevice::ReadOnly) ;
    QString corpo = res.readAll() ;
    res.close();

    corpo.replace( QRegExp("<!--mese-->") , mese ) ;
    corpo.replace( QRegExp("<!--file_copertina-->") , copertina ) ;

    pagina_anno.append( " " ) ;
    pagina_anno.append( corpo ) ;
}

void BrowserCopertine::closeListaCopertine()
{
    QFile res ;
    res.setFileName( ":/html/html/fine_lista_copertine.html" );

    res.open(QIODevice::ReadOnly) ;
    QString fine = res.readAll() ;
    res.close();

    this->pagina_anno.append( " " ) ;
    this->pagina_anno.append( fine ) ;

    //qDebug() << pagina_anno ;

    ui->MostraRiviste->setHtml( pagina_anno );
}


void BrowserCopertine::moveToMese( const QString &mese )
{
    QUrl url = ui->MostraRiviste->source() ;
    url.setFragment( mese ) ;
    ui->MostraRiviste->setSource( url );
}

void BrowserCopertine::on_itemListaAnniSelected( QListWidgetItem* item )
{
    QString anno = item->text() ;
    this->showAnno( anno ) ;
}


void BrowserCopertine::openNumeroRivista( QString copertina , QString numero , QString mese , QString anno )
{
    QFile res ;
    res.setFileName( ":/html/html/modello_rivista.html" ) ;
    res.open(QIODevice::ReadOnly) ;

    this->rivista = QString::fromUtf8( res.readAll() ) ;

    res.close();

   this->rivista.replace( QRegExp( "<!--numero-->" ) , numero ) ;
   this->rivista.replace( QRegExp( "<!--mese-->" ) , mese ) ;
   this->rivista.replace( QRegExp( "<!--anno-->" ) , anno ) ;
   this->rivista.replace( QRegExp( "<!--copertina-->" ) , copertina ) ;
}

void BrowserCopertine::appendArticolo( QString titolo , QString abstract , QString autori , QString id )
{
    QFile res ;
    res.setFileName( ":/html/html/modello_articolo.html" ) ;
    res.open( QIODevice::ReadOnly ) ;

    QString articolo = QString::fromUtf8( res.readAll() ) ;

    res.close();

    articolo.replace( QRegExp("<!--titolo-->") , titolo ) ;
    articolo.replace( QRegExp("<!--autori-->") , autori ) ;
    articolo.replace( QRegExp("<!--abstract-->") , abstract ) ;
    articolo.replace( QRegExp("<!--id-->") , id ) ;

    this->rivista.replace( QRegExp( "<!--prossimo_articolo-->" ) , articolo ) ;
}

void BrowserCopertine::closeRivista()
{
    //qDebug() << rivista ;

    ui->MostraNumeroRivista->setHtml( this->rivista );
}

void BrowserCopertine::on_rivistaClicked( const QUrl &url )
{
    QString mese = url.toString() ;
    mese.replace( QRegExp("#") ,"" ) ;

    qDebug() << mese << " " << this->anno_c ;
    this->showRivista( mese , this->anno_c ) ;
}

void BrowserCopertine::setCurrentNumero( int nr )
{
    ui->SelezioneNumero->setValue( nr );
}

void BrowserCopertine::on_indiceArticoliClicked( const QUrl &url )
{
    QString id ;
    id = url.toString() ;
    QRegExp reg_pdf("^#PDF_") ;
    QRegExp reg_bk("^#BK_") ;

    if ( reg_pdf.lastIndexIn( id ) > -1 )
    {
        id.replace( QRegExp("^#PDF_") , "" ) ;
        emit sig_openPDF ( id.toInt() ) ;
    }
    else if ( reg_bk.lastIndexIn( id ) > -1 )
    {
       id.replace( QRegExp("^#BK_") , "" ) ;
       emit sig_addBookmark ( id.toInt() ) ;
    }
}

void BrowserCopertine::on_spinNuovoNumero( const QString &numero )
{
    this->showMeseAnno( numero ) ;
}
