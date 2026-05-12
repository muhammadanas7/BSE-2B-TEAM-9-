#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QString>
#include <QStringList>
#include "Member.h"
#include "Trainer.h"
#include "MembershipPlan.h"
#include "Attendance.h"
#include "Equipment.h"
#include "Payment.h"
#include "UserAuth.h"
#include "Workout.h"
#include "Session.h"
#include "DietPlan.h"

class FileManager {
private:
    QString dataDir;
    QString membersFile;
    QString trainersFile;
    QString plansFile;
    QString attendanceFile;
    QString equipmentFile;
    QString paymentsFile;
    QString usersFile;
    QString workoutsFile;
    QString sessionsFile;
    QString dietPlansFile;

    bool        ensureDataDir();
    bool        writeLines(const QString& filePath, const QStringList& lines);
    QStringList readLines(const QString& filePath);

public:
    explicit FileManager(const QString& dataDirectory = "data");

    // Members
    bool    saveMember(const Member& m);
    bool    updateMember(const Member& m);
    bool    deleteMember(int memberId);
    bool    deleteAllMembers();
    Member  loadMember(int memberId);
    Member* loadAllMembers(int& count);
    int     getNextMemberId();

    // Trainers
    bool     saveTrainer(const Trainer& t);
    bool     updateTrainer(const Trainer& t);
    bool     deleteTrainer(int trainerId);
    bool     deleteAllTrainers();
    Trainer  loadTrainer(int trainerId);
    Trainer* loadAllTrainers(int& count);
    int      getNextTrainerId();

    // Plans
    bool            savePlan(const MembershipPlan& p);
    bool            updatePlan(const MembershipPlan& p);
    bool            deletePlan(int planId);
    MembershipPlan* loadAllPlans(int& count);

    // Attendance
    bool        saveAttendance(const Attendance& a);
    bool        deleteAttendanceForMember(int memberId);
    bool        deleteAllAttendance();
    Attendance* loadAttendanceForMember(int memberId, int& count);
    Attendance* loadAllAttendance(int& count);
    int         getNextAttendanceId();

    // Equipment
    bool       saveEquipment(const Equipment& e);
    bool       updateEquipment(const Equipment& e);
    bool       deleteEquipment(int equipId);
    bool       deleteAllEquipment();
    Equipment* loadAllEquipment(int& count);
    int        getNextEquipmentId();

    // Payments
    bool     savePayment(const Payment& p);
    bool     deletePaymentsForMember(int memberId);
    bool     deleteAllPayments();
    Payment* loadAllPayments(int& count);
    Payment* loadPaymentsForMember(int memberId, int& count);
    int      getNextPaymentId();

    // Users
    bool     saveUser(const UserAuth& u);
    bool     updateUser(const UserAuth& u);
    bool     deleteUser(int userId);
    UserAuth loadUser(const QString& username);
    UserAuth* loadAllUsers(int& count);
    bool     userExists(const QString& username);
    bool     emailExists(const QString& email);
    int      getNextUserId();

    // Workouts
    bool     saveWorkout(const Workout& w);
    bool     updateWorkout(const Workout& w);
    bool     deleteWorkoutsForMember(int memberId);
    bool     deleteAllWorkouts();
    Workout* loadWorkoutsForMember(int memberId, int& count);
    Workout* loadAllWorkouts(int& count);
    int      getNextWorkoutId();

    // Sessions
    bool     saveSession(const Session& s);
    bool     updateSession(const Session& s);
    bool     deleteSessionsForMember(int memberId);
    bool     deleteAllSessions();
    Session* loadSessionsForMember(int memberId, int& count);
    Session* loadSessionsForTrainer(int trainerId, int& count);
    Session* loadAllSessions(int& count);
    int      getNextSessionId();

    // Diet Plans
    bool      saveDietPlan(const DietPlan& d);
    DietPlan* loadAllDietPlans(int& count);
    DietPlan  loadDietPlanByName(const QString& name);

    // Utility
    bool    backupData(const QString& backupDir);
    QString getDataDirectory() const;
    void    clearAllData();
};

#endif
