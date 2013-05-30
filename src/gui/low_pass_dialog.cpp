#include "low_pass_dialog.h"
#include "ui_low_pass_dialog.h"
#include "../filter/filter.h"

low_pass_dialog::low_pass_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::low_pass_dialog)
{
    ui->setupUi(this);
    //QPushButton ok_button = ui->buttonBox->Ok;

 //   connect(ui->buttonBox->Cancel, SIGNAL(clicked()), this, SLOT(cancel_pushed()));
    connect(ui->ok_button, SIGNAL(clicked()), this, SLOT(ok_pushed()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(cancel_pushed()));
    connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(slider_change_value()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slider_change_value()));


    ui->horizontalSlider->setMaximum(256);
    ui->horizontalSlider->setMinimum(1);
    ui->radius_value->setText(QString::number(ui->horizontalSlider->value()));
}

int low_pass_dialog::slider_change_value()
{
ui->radius_value->setText(QString::number(ui->horizontalSlider->value()));
std::cout << ui->horizontalSlider->value();
return ui->horizontalSlider->value();
}

void low_pass_dialog::cancel_pushed()
{
close();
}

int low_pass_dialog::ok_pushed()
{
    close();
return 1;
}

int low_pass_dialog::get_parameter()
{
return ui->horizontalSlider->value();
}
low_pass_dialog::~low_pass_dialog()
{
    delete ui;
}
