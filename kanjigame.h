#ifndef KANJIGAME_H
#define KANJIGAME_H

#include <QObject>
#include <QFile>
#include <QHash>
#include <QStringList>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include "dictionary.h"

class KanjiGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString item READ item WRITE setitem NOTIFY itemChanged)
    Q_PROPERTY(QString mean READ mean WRITE setmean NOTIFY meanChanged)
    Q_PROPERTY(QStringList readingVariant READ readingVariant WRITE setreadingVariant NOTIFY readingVariantChanged)
    Q_PROPERTY(int state READ state WRITE setstate NOTIFY stateChanged)
    Q_PROPERTY(QStringList groups READ getGroups WRITE setGroups NOTIFY groupsChanged)

    QString m_item;
    QStringList m_readingVariant;
    Dictionary kanjiKun;
    QTimer timer;
    int m_rightAnswer=-1;

    void setRandomKanji();
    bool checkUsedKanji(const QString &kan);
    QStringList formAnswerVariants(const QString &rightVar, int varCount=5);
    int shakeAnswers(QStringList &ans);

    int m_state=0;

    QStringList m_groups;

    QString m_mean;

public:
    explicit KanjiGame(QObject *parent = 0);

    QString item() const;
    QStringList readingVariant() const;
    int state() const;

    QStringList getGroups() const;

    Q_INVOKABLE bool isEnable(int index) const;

    QString mean() const;

signals:

    void newItem();
    void chooseItem();
    void itemChanged(QString arg);
    void readingVariantChanged(QStringList arg);
    void stateChanged(int arg);
    void groupsChanged(QStringList arg);

    void meanChanged(QString arg);

public slots:

    void setitem(QString arg);
    void setreadingVariant(QStringList arg);
    void onTimeout();
    void choose(int index);
    void setstate(int arg);
    void setGroups(QStringList arg);
    void setEnable(int index, bool state);
    void setmean(QString arg);
};

#endif // KANJIGAME_H
