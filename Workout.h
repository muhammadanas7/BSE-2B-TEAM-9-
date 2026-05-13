#ifndef WORKOUT_H
#define WORKOUT_H
#include <QString>
#include <QDate>

class Workout {
private:
    int     workoutId;
    int     memberId;
    QString memberName;
    QDate   date;
    QString exerciseName;
    int     sets;
    int     reps;
    double  weight;
    QString notes;
    bool    completed;

public:
    Workout();
    Workout(int id, int memberId, const QString& memberName,
        const QDate& date, const QString& exercise,
        int sets, int reps, double weight, const QString& notes);

    int     getWorkoutId()    const;
    int     getMemberId()     const;
    QString getMemberName()   const;
    QDate   getDate()         const;
    QString getExerciseName() const;
    int     getSets()         const;
    int     getReps()         const;
    double  getWeight()       const;
    QString getNotes()        const;
    bool    getCompleted()    const;

    void setCompleted(bool c);
    void setSets(int s);
    void setReps(int r);
    void setWeight(double w);
    void setNotes(const QString& n);

    QString        toCSV() const;
    static Workout fromCSV(const QString& line);
};

#endif