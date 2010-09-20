#include "previewarticolo.h"
#include "ui_previewarticolo.h"

PreviewArticolo::PreviewArticolo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreviewArticolo)
{
    ui->setupUi(this);
}

PreviewArticolo::~PreviewArticolo()
{
    delete ui;
}

void PreviewArticolo::changeEvent(QEvent *e)
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
