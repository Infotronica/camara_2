#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImageEncoderSettings>
#include <QCameraInfo>

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = 0);
    ~WidgetMain();

    void listarCamaras();
    void crearCamara(QByteArray deviceName);
    void destruirCamara();
    void activarCamara(bool activar);

public slots:
    void imageCaptured(int id, const QImage &preview);

private slots:
    void on_chbCamara_clicked(bool checked);

    void on_btnTomarFoto_clicked();

    void on_btnCleanPixmap_clicked();

    void on_btnListarCamaras_clicked();

private:
    Ui::WidgetMain *ui;
};

#endif // WIDGETMAIN_H
