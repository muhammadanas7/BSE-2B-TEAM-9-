#ifndef GYMMANAGER_H
#define GYMMANAGER_H
#include "FileManager.h"

class GymManager {
private:
    FileManager* fileManager;

public:
    explicit GymManager(const QString& dataDir = "data");
    ~GymManager();

    // Members
    bool   registerMember(Member& m);
    bool   updateMember(const Member& m);
    bool   deactivateMember(int memberId);
    bool   deleteMember(int memberId);
    Member getMember(int memberId);
    Member* searchMembersByName(const QString& name, int& count);
    Member* getActiveMembersList(int& count);
    Member* getAllMembers(int& count);

    // Trainers
    bool    addTrainer(Trainer& t);
    bool    updateTrainer(const Trainer& t);
    bool    removeTrainer(int trainerId);
    Trainer getTrainer(int trainerId);
    Trainer* getAllTrainers(int& count);
    Trainer* getAvailableTrainers(int& count);
    bool    assignTrainerToMember(int trainerId, int memberId);

    // Plans
    bool    addPlan(MembershipPlan& plan);
    bool    updatePlan(const MembershipPlan& plan);
    MembershipPlan* getAllPlans(int& count);
    MembershipPlan  getPlanByName(const QString& name);

    // Attendance
    bool       checkIn(int memberId, const QString& session, int trainerId = -1);
    bool       checkOut(int memberId);
    Attendance* getMemberAttendance(int memberId, int& count);
    Attendance* getTodayAttendance(int& count);
    int         getMemberMonthlyAttendance(int memberId, int month, int year);

    // Equipment
    bool       addEquipment(Equipment& e);
    bool       updateEquipment(const Equipment& e);
    bool       removeEquipment(int equipId);
    Equipment* getAllEquipment(int& count);
    Equipment* getEquipmentNeedingMaintenance(int& count);

    // Payments
    bool     recordPayment(Payment& p);
    Payment* getMemberPayments(int memberId, int& count);
    Payment* getAllPayments(int& count);
    double   getTotalRevenueForMonth(int month, int year);
    double   getPendingFeesForMember(int memberId);

    // Workouts
    bool     addWorkout(Workout& w);
    bool     updateWorkout(const Workout& w);
    Workout* getMemberWorkouts(int memberId, int& count);
    Workout* getAllWorkouts(int& count);
    QString  getWorkoutSummary(int memberId);

    // Sessions
    bool     scheduleSession(Session& s);
    bool     updateSession(const Session& s);
    Session* getMemberSessions(int memberId, int& count);
    Session* getTrainerSessions(int trainerId, int& count);
    Session* getTodaySessions(int& count);
    Session* getAllSessions(int& count);
    QString  getSessionSchedule(int memberId);

    // Diet Plans
    DietPlan getDietPlan(const QString& name);
    QString  getDietPlanDetails(const QString& name);

    // Reports
    QString generateMemberReport(int memberId);
    QString generateMonthlyRevenueReport(int month, int year);
    QString generateAttendanceReport(int month, int year);
    QString generateEquipmentReport();

    // Dashboard stats
    int    getTotalMembers();
    int    getActiveMembers();
    int    getTotalTrainers();
    double getMonthlyRevenue();
    int    getTodayCheckIns();

    // Utility
    void clearAllData();   // NEW – wipes all CSVs at once
};

#endif // GYMMANAGER_H