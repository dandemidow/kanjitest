#include "kanjigame.h"

void KanjiGame::setRandomKanji()
{
    try
    {
        int tableSize = kanjiKun.size();
        if(tableSize<=0) throw std::runtime_error("empty kanji table");
        int shift = qrand()%tableSize;
        setitem(kanjiKun.kanji(shift));
        setmean(kanjiKun.mean(shift));
        setreadingVariant(formAnswerVariants(kanjiKun.reading(shift)));
    }
    catch(std::exception &e)
    {
        qWarning()<<"somthing wrong "<<e.what();
    }
}

bool KanjiGame::checkUsedKanji(const QString &kan)
{
    static QSet<QString> alreadyUsed;
    if(alreadyUsed.contains(kan)) return false;
    else
    {
        alreadyUsed.insert(kan);
        qDebug()<<"current - "<<kan;
        if(alreadyUsed.size()==kanjiKun.size())
        {
            qDebug()<<"mou ichido! ";
            alreadyUsed.clear();
        }
    }
    return true;
}

QStringList KanjiGame::formAnswerVariants(const QString& rightVar, int varCount)
{
    QStringList ans(rightVar);
    int kanjiSize = kanjiKun.size();
    if(kanjiSize-1<varCount) varCount = kanjiSize-1;
    for(int i=0; i<varCount; ++i)
    {
        int shift = qrand()%kanjiSize;
        auto reading = kanjiKun.reading(shift);
        if ( reading.size() == 0 ) throw std::runtime_error("no reading");
        if(!ans.contains(reading))
        {
            ans.append(reading);
        }
        else --i;
    }
    m_rightAnswer = shakeAnswers(ans);
    qWarning()<<ans;
    return ans;
}

int KanjiGame::shakeAnswers(QStringList &ans)
{
    QString rightAns = ans.first();
    ans.removeFirst();
    int shift = qrand()%ans.size();
    ans.insert(shift, rightAns);
    return shift;
}

KanjiGame::KanjiGame(QObject *parent) :
    QObject(parent)
{
    QStringList groups;
    qDebug()<<Q_FUNC_INFO<<kanjiKun.getGroups();
    for(auto gr: kanjiKun.getGroups())
    {
        groups.append(QString::number(gr));
    }
    setGroups(groups);
    qsrand(QTime::currentTime().msec());
    timer.setInterval(1*01*1000);
    connect(&timer, SIGNAL(timeout()), SLOT(onTimeout()));
//    timer.start();
    setRandomKanji();
    checkUsedKanji(m_item);
}

QString KanjiGame::item() const
{
    return m_item;
}

QStringList KanjiGame::readingVariant() const
{
    return m_readingVariant;
}

int KanjiGame::state() const
{
    return m_state;
}

QStringList KanjiGame::getGroups() const
{
    return m_groups;
}

bool KanjiGame::isEnable(int index) const
{
    if(index>=0&&index<m_groups.size())
    {
        return kanjiKun.isEnableGroup(m_groups.at(index).toInt());
    }
    return false;
}

QString KanjiGame::mean() const
{
    return m_mean;
}

void KanjiGame::setitem(QString arg)
{
    if (m_item != arg) {
        m_item = arg;
        emit itemChanged(arg);
    }
}

void KanjiGame::setreadingVariant(QStringList arg)
{
    if (m_readingVariant != arg) {
        m_readingVariant = arg;
        emit readingVariantChanged(arg);
    }
}

void KanjiGame::onTimeout()
{
    do
        setRandomKanji();
    while(!checkUsedKanji(m_item));
    emit newItem();
    timer.stop();
}

void KanjiGame::choose(int index)
{
    if(index>=0&&index<m_readingVariant.size())
    {
        if(m_rightAnswer==index)
        {
            setstate(0);
            emit chooseItem();
            timer.start();
        }
        else
            setstate(1);
    }
}

void KanjiGame::setstate(int arg)
{
    if (m_state != arg) {
        m_state = arg;
        emit stateChanged(arg);
    }
}

void KanjiGame::setGroups(QStringList arg)
{
    if (m_groups != arg) {
        m_groups = arg;
        emit groupsChanged(arg);
    }
}

void KanjiGame::setEnable(int index, bool state)
{
    if(index>=0&&index<m_groups.size())
    {
        qDebug()<<"group set/unset "<<index<<" state "<<state;
        if(state) kanjiKun.enableGroup(m_groups.at(index).toInt());
        else kanjiKun.disableGroup(m_groups.at(index).toInt());
    }
}

void KanjiGame::setmean(QString arg)
{
    if (m_mean != arg) {
        m_mean = arg;
        emit meanChanged(arg);
    }
}
