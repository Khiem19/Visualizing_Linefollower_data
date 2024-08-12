#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainterPath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isReadingData = false;

    QAction *onAction = new QAction(tr("&ON"), this);
    QAction *offAction = new QAction(tr("&OFF"), this);
    ui->menubar->addAction(onAction);
    ui->menubar->addAction(offAction);

    connect(onAction, &QAction::triggered, this, &MainWindow::on_actionON_triggered);
    connect(offAction, &QAction::triggered, this, &MainWindow::on_actionOFF_triggered);

    graphicsView = new QGraphicsView(this);
    graphicsView->setGeometry(0, 30, width(), height() - 30);

    graphicsScene = new QGraphicsScene(this);

    // Create a QLabel to hold the GIF movie
    bgMovie = new QMovie("D:/Studia/SEM_6/WDS/line_follower/movingroad.gif");
    bgLabel = new QLabel();
    bgLabel->setMovie(bgMovie);
    bgLabel->setScaledContents(true);
    bgMovie->start();

    // Add the QLabel to a QGraphicsProxyWidget and add that to the scene
    bgItem = graphicsScene->addWidget(bgLabel);
    bgItem->setPos(0, 0);

    // Load the overlay image
    overlayImage.load("D:/Studia/SEM_6/WDS/line_follower/model_small.png");

    // Create a QGraphicsPixmapItem for the model_small image
    overlayItem = new QGraphicsPixmapItem(QPixmap::fromImage(overlayImage));
    overlayItem->setPos((width() - overlayImage.width()) / 2, (height() - overlayImage.height()) / 2 + 30);
    graphicsScene->addItem(overlayItem);

    graphicsView->setScene(graphicsScene);

    setCentralWidget(graphicsView);

    for (int i = 0; i < 5; i++) {
        QProgressBar *progressBar = new QProgressBar(this);
        progressBar->setOrientation(Qt::Vertical);
        progressBar->setRange(0, 1000);
        progressBars.push_back(progressBar);

        QLabel *valueLabel = new QLabel(this);
        valueLabel->setAlignment(Qt::AlignCenter);

        QFont font("Arial", 12, QFont::Bold);
        QColor textColor(Qt::black);
        valueLabel->setFont(font);
        valueLabel->setStyleSheet("color: " + textColor.name());

        QColor bgColor(Qt::green);
        valueLabel->setAutoFillBackground(true);
        QPalette pal = valueLabel->palette();
        pal.setColor(QPalette::Window, bgColor);
        valueLabel->setPalette(pal);

        valueLabels.push_back(valueLabel);
    }

    serial = new QSerialPort(this);
    serial->setPortName("COM4");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    }
}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    graphicsView->setGeometry(0, ui->menubar->height(), width(), height() - ui->menubar->height());
    graphicsScene->setSceneRect(0, 0, width(), height() - ui->menubar->height());

    QLabel *movieLabel = qobject_cast<QLabel*>(bgItem->widget());
    if (movieLabel) {
        movieLabel->resize(event->size());
    }

    // Resize and reposition the overlay image to fit the window size
    if (overlayItem) {
        overlayItem->setPixmap(QPixmap::fromImage(overlayImage.scaled(graphicsScene->sceneRect().size().toSize())));
        overlayItem->setPos((graphicsScene->width() - overlayItem->pixmap().width()) / 2, (graphicsScene->height() - overlayItem->pixmap().height()) / 2 + 30); // Center the image
    }

    int w = width();
    int h = height();

    int progressBarWidth = w * 0.05;
    int progressBarHeight = h * 0.3;
    int labelWidth = progressBarWidth;
    int labelHeight = h * 0.05;
    int distanceBetween = w * 0.03;

    for (int i = 0; i < progressBars.size(); i++) {
        int x = distanceBetween + i * (progressBarWidth + distanceBetween) + w * 0.3;
        int progressBarY = h * 0.1;
        progressBars[i]->setGeometry(x, progressBarY, progressBarWidth, progressBarHeight);

        int labelY = (progressBarY + progressBarHeight + h * 0.02);
        valueLabels[i]->setGeometry(x, labelY, labelWidth, labelHeight);
    }
}

void MainWindow::on_actionON_triggered()
{
    if (!isReadingData) {
        isReadingData = true;

        if (!serial->isOpen()) {
            if (serial->open(QIODevice::ReadWrite)) {
                connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
            } else {
                qDebug() << "Failed to open serial port";
            }
        }

        // Start playing the GIF only if it's not already playing
        if (!isGifPlaying) {
            bgMovie->jumpToFrame(0);  // Restart the GIF from the beginning
            bgMovie->start();
            isGifPlaying = true;
        }
    }
}

void MainWindow::on_actionOFF_triggered()
{
    if (isReadingData) {
        isReadingData = false;

        if (serial->isOpen()) {
            disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
            serial->close();
        }
    }

    // Stop playing the GIF when the button is pressed and reset the flag
    if (isGifPlaying) {
        bgMovie->stop();
        isGifPlaying = false;
    }

    for (int i = 0; i < progressBars.size(); i++) {
        progressBars[i]->setValue(0);
        valueLabels[i]->setText(QString::number(0));
    }
}


void MainWindow::readSerialData()
{
    if (!isReadingData) {
        return;
    }

    if (serial->canReadLine()) {
        QByteArray data = serial->readLine().trimmed();
        QList<QByteArray> sensorValues = data.split('\t');

        if (sensorValues.size() == 5) {
            int index = 0;
            int maxValue = 0;
            int maxIndex = 0;
            for (const QByteArray &sensorValue : sensorValues) {
                bool ok;
                int value = sensorValue.toInt(&ok);
                if (ok) {
                    progressBars[index]->setValue(value);
                    valueLabels[index]->setText(QString::number(value));
                    if (value > maxValue) {
                        maxValue = value;
                        maxIndex = index;
                    }
                }
                index++;
            }
            for (int i = 0; i < valueLabels.size(); i++) {
                QColor bgColor(Qt::yellow);
                valueLabels[i]->setStyleSheet("background-color: " + bgColor.name());
            }
            QColor highlightColor(Qt::green);
            valueLabels[maxIndex]->setStyleSheet("background-color: " + highlightColor.name());
        }
    }
}
