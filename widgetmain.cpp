#include "widgetmain.h"
#include "ui_widgetmain.h"

QCamera *camara;
QCameraViewfinder *camaraViewFinder;
QCameraImageCapture *camaraImageCapture;
QImageEncoderSettings imageEncoderSettings;

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    listarCamaras();
}

WidgetMain::~WidgetMain()
{
    activarCamara(false);
    delete ui;
}

void WidgetMain::listarCamaras()
{
    ui->cbxCamaras->clear();
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        ui->cbxCamaras->addItem(cameraInfo.description(), cameraInfo.deviceName());
    }
}

void WidgetMain::crearCamara(QByteArray deviceName)
{
    destruirCamara();
    camara = new QCamera(deviceName,this);
    camaraViewFinder = new QCameraViewfinder(this);
    camaraImageCapture = new QCameraImageCapture(camara,this);
    camara->setViewfinder(camaraViewFinder);
    camaraViewFinder->setVisible(false);
    camara->setCaptureMode(QCamera::CaptureStillImage);
    camara->exposure()->setExposureMode(QCameraExposure::ExposureNightPortrait);
    camara->exposure()->setExposureCompensation(-1);
    camaraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
}

void WidgetMain::destruirCamara()
{
    delete camaraViewFinder;
    delete camaraImageCapture;
    delete camara;
    camaraViewFinder=nullptr;
    camaraImageCapture=nullptr;
    camara=nullptr;
}

void WidgetMain::activarCamara(bool activar)
{
    QByteArray deviceName;

    if (activar)
    {
        deviceName=ui->cbxCamaras->currentData().toByteArray();
        crearCamara(deviceName);
        ui->verticalLayoutCamara->addWidget(camaraViewFinder);
        connect(camaraImageCapture,&QCameraImageCapture::imageCaptured,this,&WidgetMain::imageCaptured);
        camara->start();
        camaraViewFinder->show();
    }
    else if (camara)
    {
        ui->verticalLayoutCamara->removeWidget(camaraViewFinder);
        disconnect(camaraImageCapture,&QCameraImageCapture::imageCaptured,this,&WidgetMain::imageCaptured);
        camara->stop();
        camaraViewFinder->hide();
        destruirCamara();
    }
}

void WidgetMain::imageCaptured(int id, const QImage &preview)
{
    QPixmap pixmap;
    int w;

    Q_UNUSED(id);
    w=ui->labelPixmapCapturada->width();
    pixmap=QPixmap::fromImage(preview);
    pixmap=pixmap.scaledToWidth(w);
    ui->labelPixmapCapturada->setPixmap(pixmap);
}

void WidgetMain::on_chbCamara_clicked(bool checked)
{
    activarCamara(checked);
}

void WidgetMain::on_btnTomarFoto_clicked()
{
    camara->searchAndLock();
    camaraImageCapture->capture();
    camara->unlock();
}

void WidgetMain::on_btnCleanPixmap_clicked()
{
    QPixmap pixmap;

    ui->labelPixmapCapturada->setPixmap(pixmap);
}

void WidgetMain::on_btnListarCamaras_clicked()
{
    listarCamaras();
}
