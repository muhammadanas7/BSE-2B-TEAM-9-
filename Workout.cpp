#include "Workout.h"
#include <QStringList>

Workout::Workout()
    : workoutId(0), memberId(0), sets(0), reps(0), weight(0.0), completed(false) {
}

Workout::Workout(int id, int memId, const QString& memName,
    const QDate& d, const QString& exercise,
    int s, int r, double w, const QString& n)
    : workoutId(id), memberId(memId), memberName(memName),
    date(d), exerciseName(exercise), sets(s), reps(r),
    weight(w), notes(n), completed(false) {
}

int     Workout::getWorkoutId()    const { return workoutId; }
int     Workout::getMemberId()     const { return memberId; }
QString Workout::getMemberName()   const { return memberName; }
QDate   Workout::getDate()         const { return date; }
QString Workout::getExerciseName() const { return exerciseName; }
int     Workout::getSets()         const { return sets; }
int     Workout::getReps()         const { return reps; }
double  Workout::getWeight()       const { return weight; }
QString Workout::getNotes()        const { return notes; }
bool    Workout::getCompleted()    const { return completed; }

void Workout::setCompleted(bool c) { completed = c; }
void Workout::setSets(int s) { sets = s; }
void Workout::setReps(int r) { reps = r; }
void Workout::setWeight(double w) { weight = w; }
void Workout::setNotes(const QString& n) { notes = n; }

QString Workout::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10")
        .arg(workoutId).arg(memberId).arg(memberName)
        .arg(date.toString("yyyy-MM-dd"))
        .arg(exerciseName)
        .arg(sets).arg(reps).arg(weight)
        .arg(notes)
        .arg(completed ? 1 : 0);
}

Workout Workout::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 10) return Workout();
    Workout w;
    w.workoutId = f[0].toInt();
    w.memberId = f[1].toInt();
    w.memberName = f[2];
    w.date = QDate::fromString(f[3], "yyyy-MM-dd");
    w.exerciseName = f[4];
    w.sets = f[5].toInt();
    w.reps = f[6].toInt();
    w.weight = f[7].toDouble();
    w.notes = f[8];
    w.completed = f[9].toInt() == 1;
    return w;
}