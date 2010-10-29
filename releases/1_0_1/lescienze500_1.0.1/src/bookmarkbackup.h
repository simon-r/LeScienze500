#ifndef BOOKMARKBACKUP_H
#define BOOKMARKBACKUP_H

#include <QDialog>

namespace Ui {
    class BookmarkBackup;
}

class BookmarkBackup : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkBackup(QWidget *parent = 0);
    ~BookmarkBackup();

private:
    Ui::BookmarkBackup *ui;
};

#endif // BOOKMARKBACKUP_H
