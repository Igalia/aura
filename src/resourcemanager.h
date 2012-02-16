#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <QObject>
#include <policy/resource.h>

class ResourceManager : public QObject {
    Q_OBJECT
public:
    static ResourceManager *instance();
    bool acquirePlaybackResources();
    bool acquireRecordingResources();
    bool releaseResources();

signals:
    void resourcesLost();

private slots:
    void denied();
    void lost();
    void granted();

private:
    ResourceManager(QObject *parent = 0);

    ResourcePolicy::ResourceSet *resources;
    bool result;
    bool waiting;

};
#endif
