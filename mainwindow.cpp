#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int tabIndex = ui->tabWidget->currentIndex();
    if(ui->tabWidget->tabText(tabIndex) == "Data usage")
        realtimeusage();
    setWindowTitle("Speedometer");
}
void MainWindow::realtimeusage(){
    QFile file("/home/tux/hei");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QDate CurrentDate = QDate::currentDate();
    unsigned long long int current_month_usage = 0, last_30day_usage = 0, total_used = 0;
    int day = 1, currentmonth = CurrentDate.month();
    ui->data_usage_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->data_usage_table->verticalHeader()->setVisible(false);
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Date")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Send")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Recive")));
    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList d = line.split(";");
        if(d[0] == "d"){
            if(d[7] == "1\n"){
                QDateTime unixdate;
                unixdate.setTime_t(d[2].toInt());
                double tx = d[3].toInt() + double(d[5].toInt())/1024;
                double rx = d[4].toInt() + double(d[6].toInt())/1024;
                if(unixdate.toString("M").toInt() == currentmonth){
                    current_month_usage += (rx + tx);
                }
                last_30day_usage += (rx + tx);
                model->setItem(day-1, 0, new QStandardItem(unixdate.toString("dd MMMM") ));
                model->setItem(day-1, 1, new QStandardItem(QString::number(rx)));
                model->setItem(day-1, 2, new QStandardItem(QString::number(tx)));
            }
            day++;
        }
        if(d[0] == "curtx" || d[0] == "currx"){
            total_used += d[1].toInt();
        }
    }
    ui->data_usage_table->setModel(model);
    ui->Total_used->setText("Total used according to system: " + QString::number(total_used/1024) + " GB");
    ui->Last_30day_usage->setText("Last 30 day usage: " + QString::number(last_30day_usage) + " MB");
    ui->This_month_usage->setText("This month usage: " + QString::number(current_month_usage) + " MB");
}

MainWindow::~MainWindow()
{
    delete ui;
}
