#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QProgressBar>
#include <QLabel>
#include <QSerialPort>
#include <QGraphicsProxyWidget>
#include <QMovie>
#include <QGraphicsPixmapItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_actionON_triggered();
    void on_actionOFF_triggered();
    void readSerialData();

private:
    Ui::MainWindow *ui;
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QImage overlayImage;
    QVector<QProgressBar*> progressBars;
    QVector<QLabel*> valueLabels;
    QSerialPort *serial;
    bool isReadingData;
    bool isGifPlaying;
    QMovie *bgMovie;
    QLabel *bgLabel;
    QGraphicsProxyWidget *bgItem;
    QGraphicsPixmapItem *overlayItem;

};

#endif // MAINWINDOW_H
