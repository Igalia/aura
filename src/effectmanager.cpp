#include <QPointer>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include <glib.h>

#include "effectmanager.h"

static QPointer<EffectManager> instancePointer;

void
EffectManager::setup(QObject *parent)
{
    if (instancePointer.isNull()) {
        instancePointer = new EffectManager(parent);
        instancePointer->loadEffects();
        instancePointer->dumpEffects();
    }
}

EffectManager*
EffectManager::instance()
{
    if (!instancePointer) {
        instancePointer = new EffectManager;
    }

    Q_ASSERT(0 != instancePointer);

    return instancePointer;
}

EffectManager::EffectManager(QObject *parent) :
    QObject(parent)
{
}

EffectManager::~EffectManager()
{
    while (!m_effects.isEmpty())
        delete m_effects.takeFirst();
}

Effect *
EffectManager::getEffect(const QString &name) const
{
    for(int i = 0; i < m_effects.size(); i++) {
        if (name == m_effects[i]->name())
            return m_effects[i];
    }

    return 0;
}

void
EffectManager::loadEffects()
{
    const char *dir;
    const gchar * const *data_dirs;

    dir = g_get_user_data_dir();
    if (!dir) {
        loadEffectsFromDirectory(dir);
    }

    data_dirs = g_get_system_data_dirs();
    while(*data_dirs) {
        dir = *data_dirs;
        loadEffectsFromDirectory(dir);
        data_dirs++;
    }
}

void
EffectManager::loadEffectsFromDirectory(const char *dir)
{
    char *path = g_build_filename(dir, "gnome-video-effects", NULL);
    QDir directory(QString(path), QString("*.effect"));
    g_free(path);

    if (!directory.exists()) {
        qCritical() << directory.absolutePath()
                    << "doesn't exist";
        return;
    }

    QFileInfoList filist = directory.entryInfoList();
    qDebug() << "found " << filist.size() << " effects!";
    for (int i = 0; i < filist.size(); ++i) {
        loadEffectFromFile(filist[i].absoluteFilePath());
    }
}

void
EffectManager::loadEffectFromFile(const QString &path)
{
    QSettings settings(path, QSettings::IniFormat);
    QString name = settings.value("effect/name").toString();
    QString desc = settings.value("effect/PipelineDescription").toString();
    Effect *effect = new Effect(name, desc);

    m_effects.append(effect);
}

void
EffectManager::dumpEffects()
{
    for(int i = 0; i < m_effects.size(); i++) {
        qDebug() << "Effect: "
                 << m_effects[i]->name()
                 << " pipeline: "
                 << m_effects[i]->desc();
    }
}
