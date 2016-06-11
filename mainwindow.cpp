#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
          this, SLOT(datausage()));
    timer->start(10000);

    //int tabIndex = ui->tabWidget->currentIndex();
    //if(ui->tabWidget->tabText(tabIndex) == "Data usage")
        datausage();
    //else if (ui->tabWidget->tabText(tabIndex) == "Real time usage")
        realtimeusage();
    setWindowTitle("Speedometer");
}
void MainWindow::realtimeusage(){
}
void MainWindow::datausage(){
    QProcess dumber;
    dumber.setStandardOutputFile("/tmp/vnstatdump");
    dumber.start("vnstat --dumpdb -i enp2s0");
    dumber.waitForFinished();
    QFile file("/tmp/vnstatdump");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QDate CurrentDate = QDate::currentDate();
    unsigned long long int current_month_usage = 0, last_30day_usage = 0, total_used = 0;
    int day = 1, currentmonth = CurrentDate.month();
    ui->data_usage_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->data_usage_table->verticalHeader()->setVisible(false);
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Date")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Send")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Recive")));
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
                model->setItem(day-1, 2, new QStandardItem(QString::number(rx, 'f' , 2) + " MB"));
                model->setItem(day-1, 1, new QStandardItem(QString::number(tx, 'f' , 2) + " MB"));
            }
            day++;
        }
        if(d[0] == "curtx" || d[0] == "currx"){
            total_used += d[1].toInt();
        }
    }
    ui->data_usage_table->setModel(model);
    ui->data_usage_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Total_used->setText("Total used according to system: " + QString::number(total_used/1024) + " GB");
    ui->Last_30day_usage->setText("Last 30 day usage: " + QString::number(last_30day_usage) + " MB");
    ui->This_month_usage->setText("This month usage: " + QString::number(current_month_usage) + " MB");
}
MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_tabWidget_currentChanged(int index)
//{

//}
