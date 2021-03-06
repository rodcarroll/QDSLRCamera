#include "qgphotocaptureservice.h"

#include <iostream>

#include <QCameraControl>
#include <QMediaServiceProviderPlugin>

QGPhotoCaptureService::QGPhotoCaptureService(const QString &service, QObject *parent) :
    QMediaService(parent)
{

    m_captureSession = 0;
    m_cameraControl = 0;
    m_imageCaptureControl = 0;
    m_videoInputDevice = 0;

    if (service == Q_MEDIASERVICE_CAMERA) {
        m_captureSession = new QGPhotoCaptureSession(this);
        m_cameraControl = new QGPhotoCameraControl(m_captureSession);
        m_imageCaptureControl = new QGPhotoImageCaptureControl(m_captureSession);
        m_videoInputDevice = new QGPhotoVideoInputDeviceControl(m_captureSession);
        m_videoRenderer = new QGPhotoVideoRendererControl(m_captureSession);
        m_exposureControl = new QGPhotoCameraExposureControl(m_captureSession);
    }

}

QGPhotoCaptureService::~QGPhotoCaptureService()
{
    delete m_captureSession;
}

QMediaControl *QGPhotoCaptureService::requestControl(const char *name)
{
    std::cout << "Request control : " << name << "\n";
    std::cout.flush();

    if (qstrcmp(name,QCameraControl_iid) == 0)
        return m_cameraControl;

    if (qstrcmp(name, QCameraImageCaptureControl_iid) == 0)
        return m_imageCaptureControl;

    if (qstrcmp(name, QVideoDeviceSelectorControl_iid) == 0)
        return m_videoInputDevice;

    if (qstrcmp(name, QVideoRendererControl_iid) == 0)
        return m_videoRenderer;

    if (qstrcmp(name, QCameraExposureControl_iid) == 0)
        return m_exposureControl;



    return 0;
}

/*
Request control : org.qt-project.qt.metadatareadercontrol/5.0
Request control : org.qt-project.qt.mediaavailabilitycontrol/5.0
Request control : org.qt-project.qt.cameracontrol/5.0
Request control : org.qt-project.qt.cameralockscontrol/5.0
Request control : org.qt-project.qt.videodeviceselectorcontrol/5.0
Request control : org.qt-project.qt.cameraexposurecontrol/5.0
Request control : org.qt-project.qt.cameraflashcontrol/5.0
Request control : org.qt-project.qt.camerafocuscontrol/5.0
Request control : org.qt-project.qt.camerazoomcontrol/5.0
Request control : org.qt-project.qt.cameraimageprocessingcontrol/5.0
*/

void QGPhotoCaptureService::releaseControl(QMediaControl *control)
{
    m_captureSession->stopViewFinder();
    qDebug("Release control");

    if (control && control == m_cameraControl) {
        qDebug("Release m_cameraControl");
    }
    if (control && control == m_imageCaptureControl) {
        qDebug("Release m_imageCaptureControl");
    }
    if (control && control == m_videoInputDevice) {
        qDebug("Release m_videoInputDevice");
        m_captureSession->stopViewFinder();
        m_captureSession->closeDevice();
    }
}
