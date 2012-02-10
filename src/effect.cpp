#include "effect.h"

Effect::Effect(const QString &name, const QString &desc) :
	m_name(name),
	m_desc(desc)
{
}

QString
Effect::name() const
{
	return m_name;
}

QString
Effect::desc() const
{
	return m_desc;
}
