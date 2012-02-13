#ifndef EFFECT_H
#define EFFECT_H

#include <QString>

class Effect {
public:
    Effect(const QString &name, const QString &desc);

    QString name() const;
    QString desc() const;

private:
    QString m_name;
    QString m_desc;
};

#endif
