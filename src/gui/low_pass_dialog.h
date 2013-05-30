#ifndef LOW_PASS_DIALOG_H
#define LOW_PASS_DIALOG_H

#include <QDialog>

namespace Ui {
class low_pass_dialog;
}

class low_pass_dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit low_pass_dialog(QWidget *parent = 0);
    ~low_pass_dialog();

public slots:
    void cancel_pushed();
    int ok_pushed();
    int slider_change_value();
    int get_parameter();
    
private:
    Ui::low_pass_dialog *ui;
};

#endif // LOW_PASS_DIALOG_H
