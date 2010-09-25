#ifndef LSERRORMESSAGE_H
#define LSERRORMESSAGE_H

#include <QDialog>

namespace Ui {
    class LSErrorMessage;
}

class LSErrorMessage : public QDialog {
    Q_OBJECT
public:
    LSErrorMessage(QWidget *parent = 0);
    ~LSErrorMessage();

    void setHtmlMessage( QString error_name , QString message ) ;

    void showDBError() ;
    void showArticleNotFound( QString file_name ) ;
    void showReaderNotStarted() ;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LSErrorMessage *ui;
};

#endif // LSERRORMESSAGE_H
