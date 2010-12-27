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


#include "configura.h"
#include "ui_configura.h"
#include "configls500.h"
#include <QFileDialog>
#include <QRegExp>
#include <QDebug>

Configura::Configura(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Configura)
{
    ui->setupUi(this);

#ifdef Q_WS_WIN
    ui->SelectAdobeAcrobat->setDisabled(true) ;
    ui->SelectEvice->setDisabled(true) ;
    ui->SelectGV->setDisabled(true) ;
    ui->SelectOkular->setDisabled(true) ;
    ui->SelectAnyPDFappl->setDisabled(true) ;
    ui->PdfApplPath->setDisabled(true) ;
    ui->SearchPdfAppl->setDisabled(true) ;
#endif

}

Configura::~Configura()
{
    delete ui;
}

void Configura::changeEvent(QEvent *e)
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

void Configura::on_Cancel_clicked()
{
    this->setVisible( false );
}

void Configura::setConfigData()
{
    configLS500 cfg ;

    QString value ;

    value = cfg.getPDFAppl() ;

    if ( value == "desktop" )
    {
        ui->SelectDesktop->setChecked( true ) ;
    }
#ifdef Q_WS_X11
    else if ( value == "okular" )
    {
        ui->SelectOkular->setChecked( true ) ;
    }
    else if ( value == "evince" )
    {
        ui->SelectEvice->setChecked( true ) ;
    }
    else if ( value == "acroread" )
    {
        ui->SelectAdobeAcrobat->setChecked( true ) ;
    }
    else if ( value == "gv" )
    {
        ui->SelectGV->setChecked( true ) ;
    }
    else
    {
        ui->SelectAnyPDFappl->setChecked( true );
        ui->PdfApplPath->setText( QDir::toNativeSeparators(value) );
    }
#endif

    value = cfg.getPDFPath1() ;
    ui->Path1->setText( QDir::toNativeSeparators(value) );

    value = cfg.getPDFPath2() ;
    ui->Path2->setText( QDir::toNativeSeparators(value) );

    value = cfg.getDBPath() ;
    ui->DBPath->setText( QDir::toNativeSeparators(value) );

    value = cfg.getCopertinePath() ;
    ui->CopertinePath->setText( QDir::toNativeSeparators(value) );

    value = cfg.getDVD() ;
    QRegExp reg( "(yes)" ) ;
    if ( reg.indexIn( value ) > -1  )
    {
        ui->radioSelectDVD->setChecked( true );
        ui->frameSelectDirectory->setEnabled( false );
    }
    else
    {
        ui->radioSelectDVD->setChecked( false );
        ui->frameSelectDirectory->setEnabled( true );
    }
}

void Configura::writeConfigData()
{
    configLS500 cfg ;

    cfg.open();

    if (ui->SelectAdobeAcrobat->isChecked())
    {
        cfg.setPDFAppl("acroread");
    }
    else if (ui->SelectEvice->isChecked())
    {
        cfg.setPDFAppl("evince");
    }
    else if (ui->SelectGV->isChecked())
    {
        cfg.setPDFAppl("gv");
    }
    else if (ui->SelectOkular->isChecked())
    {
        cfg.setPDFAppl("okular");
    }
    else if ( ui->SelectDesktop->isChecked() )
    {
        cfg.setPDFAppl("desktop");
    }
    else
    {
        QString appl ;
        appl =  QDir::fromNativeSeparators( ui->PdfApplPath->text() ) ;
        cfg.setPDFAppl( appl );
    }

    QString val ;
    val = QDir::fromNativeSeparators( ui->Path1->text() ) ;
    cfg.setPDFPath1(val);

    val = QDir::fromNativeSeparators( ui->Path2->text() ) ;
    cfg.setPDFPath2(val);

    val = QDir::fromNativeSeparators( ui->DBPath->text() ) ;
    cfg.setDBPath( val );

    val = QDir::fromNativeSeparators( ui->CopertinePath->text() ) ;
    cfg.setCopertinePath( val );

    bool dvd = ui->radioSelectDVD->isChecked() ;

    if ( dvd )
        val = "yes" ;
    else
        val = "no" ;

    cfg.setDVD( val );

    cfg.close();
}

void Configura::on_Ok_clicked()
{
    writeConfigData() ;
    this->setVisible( false );

    emit sig_NewConfigFile() ;
}

void Configura::on_SearchDBPath_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Apri Data Base LeScienze.db"), "/", tr("Data Base files (*.db)"));

    if ( fileName.size() > 0 )
        ui->DBPath->setText( fileName );
}

void Configura::on_SearchPDFPath1_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Apri Directory: Percorso articoli 1"), "/" ,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if ( dir.size() > 0 )
    {
        //dir.append("/") ;
        ui->Path1->setText( dir );
    }
}

void Configura::on_SearchPDFPath2_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Apri Directory: Percorso articoli 2"), "/" ,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if ( dir.size() > 0 )
    {
        //dir.append("/") ;
        ui->Path2->setText( dir );

    }
}

void Configura::on_SearchPdfAppl_clicked()
{
    QString appl_name = QFileDialog::getOpenFileName(this,tr("Imposta un lettore PDF"), "/", tr("File Eseguibile (*)"));

    if ( appl_name.size() > 0 )
        ui->PdfApplPath->setText( appl_name );
}

void Configura::on_SearchCopertinePath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Apri Directory: Percorso Copertine"), "/" ,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if ( dir.size() > 0 )
    {
        //dir.append("/") ;
        ui->CopertinePath->setText( dir );
    }
}

void Configura::setCurrentTool( Configura::CfgTool cfg_tool )
{
    ui->toolBox->setCurrentIndex( cfg_tool );
}


void Configura::on_radioSelectDVD_clicked(bool checked)
{
    ui->frameSelectDirectory->setEnabled( !checked );
}

void Configura::on_radioSelectHD_clicked(bool checked)
{
     ui->frameSelectDirectory->setEnabled( checked );
}
