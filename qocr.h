#ifndef QOCR_H
#define QOCR_H

#include "run.h"
#include <QObject>
#include <QSize>
#include <QPixmap>

class QOcr: public QObject, public OCRecognizer
{
    Q_OBJECT
public:
    QOcr();
    virtual ~QOcr();
    void message(string sum1,string sum2, Mat image,bool flag);
    void falsemessage(int a);
    void setImageSize(QSize);
private:
    QSize imgsize;

    QImage MatToQImage(const cv::Mat& mat);
signals:
    void newMessage(QString sum1,QString sum2, QPixmap image,bool flag);

};

#endif // QOCR_H
