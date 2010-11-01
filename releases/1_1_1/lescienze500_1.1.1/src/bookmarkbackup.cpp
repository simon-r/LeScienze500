#include "bookmarkbackup.h"
#include "ui_bookmarkbackup.h"

BookmarkBackup::BookmarkBackup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkBackup)
{
    ui->setupUi(this);
}

BookmarkBackup::~BookmarkBackup()
{
    delete ui;
}
