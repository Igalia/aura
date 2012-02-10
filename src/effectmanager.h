#include <QObject>
#include <QList>

#include "effect.h"

class EffectManager : public QObject {
   Q_OBJECT

public:
   static EffectManager *instance();

   static void setup(QObject *parent = 0);

   virtual ~EffectManager();

private:
   EffectManager(QObject *parent = 0);

   void loadEffects();
   void loadEffectsFromDirectory(const char *dir);
   void loadEffectFromFile(const QString &path);
   void dumpEffects();

   QList<Effect *> m_effects;
};