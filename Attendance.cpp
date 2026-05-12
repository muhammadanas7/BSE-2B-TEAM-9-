#include "Attendance.h"
#include <QStringList>

Attendance::Attendance()
    : recordId(0), memberId(0), isPresent(false), trainerId(-1) {
}

Attendance::Attendance(int recId, int memId, const QString& memName,
    const QDate& d, const QTime& ci,
    const QString& session, int tId)
    : recordId(recId), memberId(memId), memberName(memName),
    date(d), checkIn(ci), isPresent(true), sessionType(session), trainerId(tId) {
}

int     Attendance::getRecordId()    const { return recordId; }
int     Attendance::getMemberId()    const { return memberId; }
QString Attendance::getMemberName()  const { return memberName; }
QDate   Attendance::getDate()        const { return date; }
QTime   Attendance::getCheckIn()     const { return checkIn; }
QTime   Attendance::getCheckOut()    const { return checkOut; }
bool    Attendance::getIsPresent()   const { return isPresent; }
QString Attendance::getSessionType() const { return sessionType; }
int     Attendance::getTrainerId()   const { return trainerId; }

void Attendance::setCheckOut(const QTime& t) { checkOut = t; }
void Attendance::setIsPresent(bool p) { isPresent = p; }
void Attendance::setSessionType(const QString& s) { sessionType = s; }
void Attendance::setTrainerId(int id) { trainerId = id; }

int Attendance::getDurationMinutes() const {
    if (!checkOut.isValid()) return 0;
    return checkIn.secsTo(checkOut) / 60;
}

QString Attendance::getDurationString() const {
    int mins = getDurationMinutes();
    if (mins <= 0) return "In Progress";
    return QString("%1h %2m").arg(mins / 60).arg(mins % 60);
}

bool Attendance::isCheckOutRecorded() const { return checkOut.isValid(); }

QString Attendance::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9")
        .arg(recordId).arg(memberId).arg(memberName)
        .arg(date.toString("yyyy-MM-dd"))
        .arg(checkIn.toString("HH:mm"))
        .arg(checkOut.isValid() ? checkOut.toString("HH:mm") : "N/A")
        .arg(isPresent ? 1 : 0)
        .arg(sessionType)
        .arg(trainerId);
}

Attendance Attendance::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 9) return Attendance();
    Attendance a;
    a.recordId = f[0].toInt();
    a.memberId = f[1].toInt();
    a.memberName = f[2];
    a.date = QDate::fromString(f[3], "yyyy-MM-dd");
    a.checkIn = QTime::fromString(f[4], "HH:mm");
    if (f[5] != "N/A") a.checkOut = QTime::fromString(f[5], "HH:mm");
    a.isPresent = f[6].toInt() == 1;
    a.sessionType = f[7];
    a.trainerId = f[8].toInt();
    return a;
}