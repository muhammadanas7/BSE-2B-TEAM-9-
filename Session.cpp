#include "Session.h"
#include <QStringList>

Session::Session()
    : sessionId(0), memberId(0), trainerId(0), status("Scheduled") {
}

Session::Session(int id, int memId, const QString& memName,
    int trId, const QString& trName,
    const QDate& date, const QTime& start, const QTime& end,
    const QString& type)
    : sessionId(id), memberId(memId), memberName(memName),
    trainerId(trId), trainerName(trName),
    sessionDate(date), startTime(start), endTime(end),
    sessionType(type), status("Scheduled") {
}

int     Session::getSessionId()   const { return sessionId; }
int     Session::getMemberId()    const { return memberId; }
QString Session::getMemberName()  const { return memberName; }
int     Session::getTrainerId()   const { return trainerId; }
QString Session::getTrainerName() const { return trainerName; }
QDate   Session::getSessionDate() const { return sessionDate; }
QTime   Session::getStartTime()   const { return startTime; }
QTime   Session::getEndTime()     const { return endTime; }
QString Session::getSessionType() const { return sessionType; }
QString Session::getStatus()      const { return status; }

void Session::setStatus(const QString& s) { status = s; }
void Session::setStartTime(const QTime& t) { startTime = t; }
void Session::setEndTime(const QTime& t) { endTime = t; }

QString Session::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10")
        .arg(sessionId).arg(memberId).arg(memberName)
        .arg(trainerId).arg(trainerName)
        .arg(sessionDate.toString("yyyy-MM-dd"))
        .arg(startTime.toString("HH:mm"))
        .arg(endTime.toString("HH:mm"))
        .arg(sessionType)
        .arg(status);
}

Session Session::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 10) return Session();
    Session s;
    s.sessionId = f[0].toInt();
    s.memberId = f[1].toInt();
    s.memberName = f[2];
    s.trainerId = f[3].toInt();
    s.trainerName = f[4];
    s.sessionDate = QDate::fromString(f[5], "yyyy-MM-dd");
    s.startTime = QTime::fromString(f[6], "HH:mm");
    s.endTime = QTime::fromString(f[7], "HH:mm");
    s.sessionType = f[8];
    s.status = f[9];
    return s;
}