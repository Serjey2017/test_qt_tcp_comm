#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void newMessage(QString);

private slots:
    void on_lineEdit_editingFinished();
    void on_pb_getID_clicked();
    void on_pb_getname_clicked();
    void on_pb_getaddress_clicked();
    void readSocket();
    void displayMessage(const QString& str);
    void displayError();



private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    const unsigned int default_port = 5000;
    const QHostAddress default_addr = QHostAddress::LocalHost;
    const int header_size = 2;

};
#endif // MAINWINDOW_H
