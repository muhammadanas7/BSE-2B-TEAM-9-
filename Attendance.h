#ifndef ATTENDANCE_H
#define ATTENDANCE_H
#include <QString>
#include <QDate>
#include <QTime>

class Attendance {
private:
    int     recordId;
    int     memberId;
    QString memberName;
    QDate   date;
    QTime   checkIn;
    QTime   checkOut;
    bool    isPresent;
    QString sessionType;
    int     trainerId;

public:
    Attendance();
    Attendance(int recId, int memId, const QString& memName,
        const QDate& date, const QTime& checkIn,
        const QString& session, int trainerId = -1);

    int     getRecordId()    const;
    int     getMemberId()    const;
    QString getMemberName()  const;
    QDate   getDate()        const;
    QTime   getCheckIn()     const;
    QTime   getCheckOut()    const;
    bool    getIsPresent()   const;
    QString getSessionType() const;
    int     getTrainerId()   const;

    void setCheckOut(const QTime& t);
    void setIsPresent(bool p);
    void setSessionType(const QString& s);
    void setTrainerId(int id);

    int     getDurationMinutes() const;
    QString getDurationString()  const;
    bool    isCheckOutRecorded() const;

    QString           toCSV() const;
    static Attendance fromCSV(const QString& line);
};

#endif