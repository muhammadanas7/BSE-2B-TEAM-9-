#ifndef SESSION_H
#define SESSION_H
#include <QString>
#include <QDate>
#include <QTime>

class Session {
private:
    int     sessionId;
    int     memberId;
    QString memberName;
    int     trainerId;
    QString trainerName;
    QDate   sessionDate;
    QTime   startTime;
    QTime   endTime;
    QString sessionType;
    QString status;

public:
    Session();
    Session(int id, int memberId, const QString& memberName,
        int trainerId, const QString& trainerName,
        const QDate& date, const QTime& start, const QTime& end,
        const QString& type);

    int     getSessionId()   const;
    int     getMemberId()    const;
    QString getMemberName()  const;
    int     getTrainerId()   const;
    QString getTrainerName() const;
    QDate   getSessionDate() const;
    QTime   getStartTime()   const;
    QTime   getEndTime()     const;
    QString getSessionType() const;
    QString getStatus()      const;

    void setStatus(const QString& s);
    void setStartTime(const QTime& t);
    void setEndTime(const QTime& t);

    QString        toCSV() const;
    static Session fromCSV(const QString& line);
};

#endif