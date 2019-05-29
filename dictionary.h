#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QHash>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

struct KanjiDescriptor
{
    QString kanji;
    QString reading;
    QString mean;

    KanjiDescriptor(const QString &kj, const QString &rd, const QString &mn);
};

class Dictionary
{
    class Group
    {
        friend class Dictionary;
        int id=0;
        bool enable=false;
        QHash<QString, KanjiDescriptor> dict;
    public:
        explicit Group();
        Group(int ID);
        void setEnable(bool en);
        void add(const KanjiDescriptor &kanji);
        int size() const;
    };

    QHash<int, Group> groups;

    void addGroup(int id);
    void addKanji(int group, const KanjiDescriptor &kanji);
    KanjiDescriptor getKanji(int globalNum);
    QString fileNameToGroup(const QString &fileName);
    void loadDict();
    QStringList getAllDictFile();
    bool addToDictionary(int id, const QString &line);

public:
    Dictionary();
    QList<int> getGroups() const;
    bool isEnableGroup(int id) const;
    bool enableGroup(int group);
    bool disableGroup(int group);
    int size();
    QString kanji(int i);
    QString reading(int i);
    QString mean(int i);
};

#endif // DICTIONARY_H
