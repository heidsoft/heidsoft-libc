#include <qmainwindow.h>
#include <qlineedit.h>
#include <qstring.h>

class LineEdit : public QMainWindow {
    Q_OBJECT

public:
    LineEdit(QWidget* parent = 0, const char* name = 0);
    QLineEdit* username_entry;
    QLineEdit* password_entry;

private slots:
    void Clicked();
};


