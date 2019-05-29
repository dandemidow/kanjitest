#include "dictionary.h"
#include <algorithm>

void Dictionary::addGroup(int id)
{
    if(!groups.contains(id))
        groups.insert(id, Group(id));
}

QList<int> Dictionary::getGroups() const
{
    Q_ASSERT(!groups.keys().isEmpty());
    auto keys = groups.keys();
    std::sort(keys.begin(), keys.end());
    return keys;
}

bool Dictionary::isEnableGroup(int id) const
{
    if(groups.contains(id))
    {
        return groups[id].enable;
    }
    return false;
}

void Dictionary::addKanji(int group, const KanjiDescriptor &kanji)
{
    if(groups.contains(group))
    {
        qWarning()<<"add k "<<kanji.mean;
        groups[group].add(kanji);
    }
    else
        qWarning()<<"group is not exist";
}

KanjiDescriptor Dictionary::getKanji(int globalNum)
{
    for(auto &grp :groups)
    {
        if(!grp.enable) continue;
        if(globalNum>=grp.size()) globalNum-=grp.size();
        else
        {
            auto it = grp.dict.begin();
            it = it+globalNum;
            return *(it);
        }
    }
    qFatal("length mismatch");
    return KanjiDescriptor("","","");
}

QString Dictionary::fileNameToGroup(const QString &fileName)
{
    QString number(fileName);
    number.remove(QRegExp("l"));
    number.remove(QRegExp("\.kj"));
    return number;
}

void Dictionary::loadDict()
{
    QStringList files(getAllDictFile());
    for(auto &file: files)
    {
        qDebug()<<"load: "<<file<<"group:"<<fileNameToGroup(file);
        int groupID = fileNameToGroup(file).toInt();
        addGroup(groupID);
        QFile dict(file);
        if(dict.open(QFile::ReadOnly|QFile::Text))
        {
            QTextStream fileStream(&dict);
            fileStream.setCodec("UTF-8");
            while(addToDictionary(groupID, fileStream.readLine()));
            groups[groupID].setEnable((groupID-1)?false:true);
        }
    }
}

QStringList Dictionary::getAllDictFile()
{
    return QDir().entryList(QStringList()<<"l*.kj");
}

bool Dictionary::addToDictionary(int id, const QString &line)
{
    if(line.isEmpty()) return false;
    QString kj, reading, mean;
    QString tmp(line);
    QTextStream stm(&tmp);
    stm>>kj>>reading;
    mean.append(stm.readAll());
    if(!kj.isEmpty())
    {
        addKanji(id, KanjiDescriptor(kj, reading, mean));
        return true;
    }
    return false;
}

Dictionary::Dictionary()
{
    loadDict();
}

bool Dictionary::enableGroup(int group)
{
    if(groups.contains(group))
    {
        groups[group].setEnable(true);
        return true;
    }
    return false;
}

bool Dictionary::disableGroup(int group)
{
    if(groups.contains(group))
    {
        groups[group].setEnable(false);
        return true;
    }
    return false;
}

int Dictionary::size()
{
    int sz=0;
    for(auto &grp :groups)
    {
        sz+=grp.enable?grp.size():0;
    }
    return sz;
}

QString Dictionary::kanji(int i)
{
    if(i>=0&&i<size())
    {
        return getKanji(i).kanji;
    }
    return QString();
}

QString Dictionary::reading(int i)
{
    return getKanji(i).reading;
}

QString Dictionary::mean(int i)
{
    return getKanji(i).mean;
}

void Dictionary::Group::setEnable(bool en)
{
    enable = en;
}

Dictionary::Group::Group()
{
}

Dictionary::Group::Group(int ID): id(ID)
{
}

void Dictionary::Group::add(const KanjiDescriptor &kanji)
{
    dict.insert(kanji.kanji, kanji);
}

int Dictionary::Group::size() const
{
    return dict.size();
}



KanjiDescriptor::KanjiDescriptor(const QString &kj, const QString &rd, const QString &mn):
    kanji(kj), reading(rd), mean(mn) {}
