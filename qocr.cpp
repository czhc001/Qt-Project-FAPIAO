#include "qocr.h"
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QImage>

QOcr::QOcr()
{
    //qRegisterMetaType<QPixmap>("QPixmap");
    QString path = QCoreApplication::applicationDirPath();
    QString model0 = "/models/lenet.prototxt";
    QString model1 = "/models/lenet_iter_10000.caffemodel";
    QString modelpath0 = path + model0;
    QString modelpath1 = path + model1;
    qDebug() << "P0:: " << modelpath0;
    qDebug() << "P1:: " << modelpath1;
    QByteArray ba0 = modelpath0.toLocal8Bit();
    QByteArray ba1 = modelpath1.toLocal8Bit();
    char * p0 = ba0.data();
    char * p1 = ba1.data();
    //qDebug() << "std0" << p0;
    //qDebug() << "std1" << p1;
    //qDebug() << "1" << "2";
    set_modelpath0(p0);
    set_modelpath1(p1);
}

QOcr::~QOcr()
{
    qDebug() << "delete QOCR";
}

void QOcr::message(string sum1,string sum2, Mat image,bool flag){
    //std::cout << "!!! " << sum1 << " " << sum2 << std::endl;
    QString m1 = QString::fromStdString(sum1);
    QString m2 = QString::fromStdString(sum2);
    //qDebug() << QString("!!!") << m1 << QString("!!!") <<m2;
    //qDebug() << "QOcr emited";

    QImage img = MatToQImage(image);
    QPixmap pimg = QPixmap::fromImage(img);
    pimg = pimg.scaled(imgsize, Qt::KeepAspectRatio);
    emit newMessage(m1, m2, pimg, flag);
}

void QOcr::falsemessage(int a){
    qDebug() << "OCR ERROR: " << a;
}

void QOcr::setImageSize(QSize size){
    imgsize = size;
}

QImage QOcr::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

