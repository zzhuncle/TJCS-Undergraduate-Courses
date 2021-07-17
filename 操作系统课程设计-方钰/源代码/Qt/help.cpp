#include "help.h"
#include "ui_help.h"
#include "customtablestyle.h"

Help::Help(QWidget *parent) : QWidget(parent), ui(new Ui::Help)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
}

Help::~Help()
{
    delete ui;
}
