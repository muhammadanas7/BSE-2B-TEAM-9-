#include "GymManager.h"
#include <QDate>
#include <QTime>

GymManager::GymManager(const QString& dataDir) {
    fileManager = new FileManager(dataDir);
}

GymManager::~GymManager() {
    delete fileManager;
}

bool GymManager::registerMember(Member& m) {
    if (!m.getEmail().isEmpty() && !Member::isValidEmail(m.getEmail())) {
        return false;
    }
    int id = fileManager->getNextMemberId();
    double totalDue = m.getTotalDue();
    m.setPendingFees(totalDue);
    Member newM(id, m.getName(), m.getCnic(), m.getPhone(),
        m.getEmail(), m.getAddress(), m.getGender(),
        m.getDateOfBirth(), m.getMembershipPlan(),
        m.getWantsTrainer(), m.getDietPlan(), m.getAssignedTrainerId());
    newM.setPendingFees(totalDue);
    m = newM;
    return fileManager->saveMember(m);
}

bool GymManager::updateMember(const Member& m) {
    return fileManager->updateMember(m);
}

bool GymManager::deactivateMember(int memberId) {
    Member m = fileManager->loadMember(memberId);
    if (m.getMemberId() == 0) return false;
    m.setIsActive(false);
    return fileManager->updateMember(m);
}

bool GymManager::deleteMember(int memberId) {
    fileManager->deleteAttendanceForMember(memberId);
    fileManager->deletePaymentsForMember(memberId);
    fileManager->deleteWorkoutsForMember(memberId);
    fileManager->deleteSessionsForMember(memberId);
    return fileManager->deleteMember(memberId);
}

Member GymManager::getMember(int memberId) {
    return fileManager->loadMember(memberId);
}

Member* GymManager::getAllMembers(int& count) {
    return fileManager->loadAllMembers(count);
}

Member* GymManager::getActiveMembersList(int& count) {
    int total = 0;
    Member* all = fileManager->loadAllMembers(total);
    if (!all || total == 0) { count = 0; return nullptr; }
    int activeCount = 0;
    for (int i = 0; i < total; ++i) if (all[i].getIsActive()) ++activeCount;
    if (activeCount == 0) { delete[] all; count = 0; return nullptr; }
    Member* active = new Member[activeCount];
    int idx = 0;
    for (int i = 0; i < total; ++i) if (all[i].getIsActive()) active[idx++] = all[i];
    delete[] all;
    count = activeCount;
    return active;
}

Member* GymManager::searchMembersByName(const QString& name, int& count) {
    int total = 0;
    Member* all = fileManager->loadAllMembers(total);
    if (!all) { count = 0; return nullptr; }
    int matchCount = 0;
    for (int i = 0; i < total; ++i)
        if (all[i].getName().toLower().contains(name.toLower())) ++matchCount;
    if (matchCount == 0) { delete[] all; count = 0; return nullptr; }
    Member* result = new Member[matchCount];
    int idx = 0;
    for (int i = 0; i < total; ++i)
        if (all[i].getName().toLower().contains(name.toLower())) result[idx++] = all[i];
    delete[] all;
    count = matchCount;
    return result;
}

bool GymManager::addTrainer(Trainer& t) {
    int id = fileManager->getNextTrainerId();
    Trainer newT(id, t.getName(), t.getPhone(), t.getEmail(),
        t.getSpecialization(), t.getShift(), t.getSalary());
    t = newT;
    return fileManager->saveTrainer(t);
}

bool GymManager::updateTrainer(const Trainer& t) {
    return fileManager->updateTrainer(t);
}

bool GymManager::removeTrainer(int trainerId) {
    return fileManager->deleteTrainer(trainerId);
}

Trainer GymManager::getTrainer(int trainerId) {
    return fileManager->loadTrainer(trainerId);
}

Trainer* GymManager::getAllTrainers(int& count) {
    return fileManager->loadAllTrainers(count);
}

Trainer* GymManager::getAvailableTrainers(int& count) {
    int total = 0;
    Trainer* all = fileManager->loadAllTrainers(total);
    if (!all) { count = 0; return nullptr; }
    int avail = 0;
    for (int i = 0; i < total; ++i) if (all[i].getIsAvailable()) ++avail;
    if (avail == 0) { delete[] all; count = 0; return nullptr; }
    Trainer* result = new Trainer[avail];
    int idx = 0;
    for (int i = 0; i < total; ++i) if (all[i].getIsAvailable()) result[idx++] = all[i];
    delete[] all;
    count = avail;
    return result;
}

bool GymManager::assignTrainerToMember(int trainerId, int memberId) {
    Trainer t = fileManager->loadTrainer(trainerId);
    if (t.getTrainerId() == 0 || !t.getIsAvailable()) return false;
    Member m = fileManager->loadMember(memberId);
    if (m.getMemberId() == 0) return false;
    m.setAssignedTrainerId(trainerId);
    fileManager->updateMember(m);
    t.incrementAssigned();
    return fileManager->updateTrainer(t);
}

bool GymManager::addPlan(MembershipPlan& plan) {
    return fileManager->savePlan(plan);
}

bool GymManager::updatePlan(const MembershipPlan& plan) {
    return fileManager->updatePlan(plan);
}

MembershipPlan* GymManager::getAllPlans(int& count) {
    return fileManager->loadAllPlans(count);
}

MembershipPlan GymManager::getPlanByName(const QString& name) {
    int count = 0;
    MembershipPlan* plans = fileManager->loadAllPlans(count);
    MembershipPlan found;
    for (int i = 0; i < count; ++i)
        if (plans[i].getPlanName() == name) { found = plans[i]; break; }
    delete[] plans;
    return found;
}

bool GymManager::checkIn(int memberId, const QString& session, int trainerId) {
    Member m = fileManager->loadMember(memberId);
    if (m.getMemberId() == 0 || !m.getIsActive()) return false;
    int recId = fileManager->getNextAttendanceId();
    Attendance a(recId, memberId, m.getName(),
        QDate::currentDate(), QTime::currentTime(), session, trainerId);
    return fileManager->saveAttendance(a);
}

bool GymManager::checkOut(int memberId) {
    int total = 0;
    Attendance* all = fileManager->loadAllAttendance(total);
    if (!all) return false;
    for (int i = 0; i < total; ++i) {
        if (all[i].getMemberId() == memberId &&
            all[i].getDate() == QDate::currentDate() &&
            !all[i].isCheckOutRecorded()) {
            all[i].setCheckOut(QTime::currentTime());
            for (int j = 0; j < total; ++j)
                fileManager->saveAttendance(all[j]);
            delete[] all;
            return true;
        }
    }
    delete[] all;
    return false;
}

Attendance* GymManager::getMemberAttendance(int memberId, int& count) {
    return fileManager->loadAttendanceForMember(memberId, count);
}

Attendance* GymManager::getTodayAttendance(int& count) {
    int total = 0;
    Attendance* all = fileManager->loadAllAttendance(total);
    if (!all) { count = 0; return nullptr; }
    QDate today = QDate::currentDate();
    int todayCount = 0;
    for (int i = 0; i < total; ++i) if (all[i].getDate() == today) ++todayCount;
    if (todayCount == 0) { delete[] all; count = 0; return nullptr; }
    Attendance* result = new Attendance[todayCount];
    int idx = 0;
    for (int i = 0; i < total; ++i) if (all[i].getDate() == today) result[idx++] = all[i];
    delete[] all;
    count = todayCount;
    return result;
}

int GymManager::getMemberMonthlyAttendance(int memberId, int month, int year) {
    int total = 0;
    Attendance* all = fileManager->loadAttendanceForMember(memberId, total);
    if (!all) return 0;
    int cnt = 0;
    for (int i = 0; i < total; ++i)
        if (all[i].getDate().month() == month &&
            all[i].getDate().year() == year && all[i].getIsPresent()) ++cnt;
    delete[] all;
    return cnt;
}

bool GymManager::addEquipment(Equipment& e) {
    int id = fileManager->getNextEquipmentId();
    Equipment newE(id, e.getName(), e.getCategory(), e.getBrand(),
        e.getQuantity(), e.getPurchasePrice(), e.getPurchaseDate());
    e = newE;
    return fileManager->saveEquipment(e);
}

bool GymManager::updateEquipment(const Equipment& e) {
    return fileManager->updateEquipment(e);
}

bool GymManager::removeEquipment(int equipId) {
    return fileManager->deleteEquipment(equipId);
}

Equipment* GymManager::getAllEquipment(int& count) {
    return fileManager->loadAllEquipment(count);
}

Equipment* GymManager::getEquipmentNeedingMaintenance(int& count) {
    int total = 0;
    Equipment* all = fileManager->loadAllEquipment(total);
    if (!all) { count = 0; return nullptr; }
    int needsMaint = 0;
    for (int i = 0; i < total; ++i) if (all[i].isMaintenanceDue()) ++needsMaint;
    if (needsMaint == 0) { delete[] all; count = 0; return nullptr; }
    Equipment* result = new Equipment[needsMaint];
    int idx = 0;
    for (int i = 0; i < total; ++i) if (all[i].isMaintenanceDue()) result[idx++] = all[i];
    delete[] all;
    count = needsMaint;
    return result;
}

bool GymManager::recordPayment(Payment& p) {
    Member m = fileManager->loadMember(p.getMemberId());
    if (m.getMemberId() == 0) return false;
    double pendingFees = m.getPendingFees();
    double amount = p.getAmount();
    if (amount <= 0) return false;
    if (amount < pendingFees) return false;
    if (amount > pendingFees) return false;
    int id = fileManager->getNextPaymentId();
    Payment newP(id, p.getMemberId(), m.getName(), amount,
        p.getPaymentDate(), p.getPaymentMethod(), p.getDescription());
    p = newP;
    m.addPayment(amount);
    fileManager->updateMember(m);
    return fileManager->savePayment(p);
}

Payment* GymManager::getMemberPayments(int memberId, int& count) {
    return fileManager->loadPaymentsForMember(memberId, count);
}

Payment* GymManager::getAllPayments(int& count) {
    return fileManager->loadAllPayments(count);
}

double GymManager::getTotalRevenueForMonth(int month, int year) {
    int total = 0;
    Payment* all = fileManager->loadAllPayments(total);
    if (!all) return 0.0;
    double revenue = 0.0;
    for (int i = 0; i < total; ++i)
        if (all[i].getPaymentDate().month() == month &&
            all[i].getPaymentDate().year() == year && all[i].getIsCleared())
            revenue += all[i].getAmount();
    delete[] all;
    return revenue;
}

double GymManager::getPendingFeesForMember(int memberId) {
    Member m = fileManager->loadMember(memberId);
    return m.getPendingFees();
}

bool GymManager::addWorkout(Workout& w) {
    int id = fileManager->getNextWorkoutId();
    Workout newW(id, w.getMemberId(), w.getMemberName(), w.getDate(),
        w.getExerciseName(), w.getSets(), w.getReps(),
        w.getWeight(), w.getNotes());
    w = newW;
    return fileManager->saveWorkout(w);
}

bool GymManager::updateWorkout(const Workout& w) {
    return fileManager->updateWorkout(w);
}

Workout* GymManager::getMemberWorkouts(int memberId, int& count) {
    return fileManager->loadWorkoutsForMember(memberId, count);
}

Workout* GymManager::getAllWorkouts(int& count) {
    return fileManager->loadAllWorkouts(count);
}

QString GymManager::getWorkoutSummary(int memberId) {
    int count = 0;
    Workout* workouts = getMemberWorkouts(memberId, count);
    if (!workouts) return "No workouts recorded.";
    QString summary = QString("Workout History for Member ID %1:\n\n").arg(memberId);
    for (int i = 0; i < count; ++i) {
        summary += QString("%1: %2 - %3 sets x %4 reps @ %5 kg %6\n")
            .arg(workouts[i].getDate().toString("dd-MMM"))
            .arg(workouts[i].getExerciseName())
            .arg(workouts[i].getSets())
            .arg(workouts[i].getReps())
            .arg(workouts[i].getWeight())
            .arg(workouts[i].getCompleted() ? "✓" : "");
    }
    delete[] workouts;
    return summary;
}

bool GymManager::scheduleSession(Session& s) {
    int id = fileManager->getNextSessionId();
    Session newS(id, s.getMemberId(), s.getMemberName(),
        s.getTrainerId(), s.getTrainerName(),
        s.getSessionDate(), s.getStartTime(), s.getEndTime(),
        s.getSessionType());
    s = newS;
    return fileManager->saveSession(s);
}

bool GymManager::updateSession(const Session& s) {
    return fileManager->updateSession(s);
}

Session* GymManager::getMemberSessions(int memberId, int& count) {
    return fileManager->loadSessionsForMember(memberId, count);
}

Session* GymManager::getTrainerSessions(int trainerId, int& count) {
    return fileManager->loadSessionsForTrainer(trainerId, count);
}

Session* GymManager::getTodaySessions(int& count) {
    int total = 0;
    Session* all = fileManager->loadAllSessions(total);
    if (!all) { count = 0; return nullptr; }
    QDate today = QDate::currentDate();
    int todayCount = 0;
    for (int i = 0; i < total; ++i) if (all[i].getSessionDate() == today) todayCount++;
    if (todayCount == 0) { delete[] all; count = 0; return nullptr; }
    Session* result = new Session[todayCount];
    int idx = 0;
    for (int i = 0; i < total; ++i) if (all[i].getSessionDate() == today) result[idx++] = all[i];
    delete[] all;
    count = todayCount;
    return result;
}

Session* GymManager::getAllSessions(int& count) {
    return fileManager->loadAllSessions(count);
}

QString GymManager::getSessionSchedule(int memberId) {
    int count = 0;
    Session* sessions = getMemberSessions(memberId, count);
    if (!sessions) return "No sessions scheduled.";
    QString schedule = QString("Session Schedule for Member ID %1:\n\n").arg(memberId);
    for (int i = 0; i < count; ++i) {
        schedule += QString("%1: %2 with %3 (%4 - %5) - %6\n")
            .arg(sessions[i].getSessionDate().toString("dd-MMM-yyyy"))
            .arg(sessions[i].getSessionType())
            .arg(sessions[i].getTrainerName())
            .arg(sessions[i].getStartTime().toString("HH:mm"))
            .arg(sessions[i].getEndTime().toString("HH:mm"))
            .arg(sessions[i].getStatus());
    }
    delete[] sessions;
    return schedule;
}

DietPlan GymManager::getDietPlan(const QString& name) {
    return fileManager->loadDietPlanByName(name);
}

QString GymManager::getDietPlanDetails(const QString& name) {
    DietPlan plan = getDietPlan(name);
    if (plan.getPlanId() == 0) return "Diet plan not found.";
    return plan.getFullPlan();
}

QString GymManager::generateMemberReport(int memberId) {
    Member m = fileManager->loadMember(memberId);
    if (m.getMemberId() == 0) return "Member not found.";
    QString trainerInfo = m.getWantsTrainer() ? "Yes (+Rs. 1000)" : "No";
    QString dietInfo = m.getDietPlan();
    if (dietInfo == "None") dietInfo = "Not selected";
    return QString(
        "====== MEMBER REPORT ======\n"
        "ID       : %1\n"
        "Name     : %2\n"
        "Plan     : %3\n"
        "Trainer  : %4\n"
        "Diet Plan: %5\n"
        "Status   : %6\n"
        "Joined   : %7\n"
        "Total Due: Rs. %8\n"
        "Fees Paid: Rs. %9\n"
        "Pending  : Rs. %10\n"
        "==========================="
    ).arg(m.getMemberId()).arg(m.getName()).arg(m.getMembershipPlan())
        .arg(trainerInfo).arg(dietInfo)
        .arg(m.getStatusString()).arg(m.getJoinDate().toString("dd-MMM-yyyy"))
        .arg(m.getTotalDue(), 0, 'f', 2)
        .arg(m.getTotalFeesPaid(), 0, 'f', 2).arg(m.getPendingFees(), 0, 'f', 2);
}

QString GymManager::generateMonthlyRevenueReport(int month, int year) {
    double rev = getTotalRevenueForMonth(month, year);
    return QString("Monthly Revenue (%1/%2): Rs. %3").arg(month).arg(year).arg(rev, 0, 'f', 2);
}

QString GymManager::generateAttendanceReport(int month, int year) {
    int total = 0;
    Attendance* all = fileManager->loadAllAttendance(total);
    int count = 0;
    for (int i = 0; i < total; ++i)
        if (all[i].getDate().month() == month && all[i].getDate().year() == year) ++count;
    delete[] all;
    return QString("Attendance for %1/%2: %3 sessions recorded.").arg(month).arg(year).arg(count);
}

QString GymManager::generateEquipmentReport() {
    int total = 0;
    Equipment* all = fileManager->loadAllEquipment(total);
    int needsMaint = 0;
    for (int i = 0; i < total; ++i) if (all[i].isMaintenanceDue()) ++needsMaint;
    delete[] all;
    return QString("Equipment: %1 total | %2 need maintenance.").arg(total).arg(needsMaint);
}

int GymManager::getTotalMembers() {
    int count = 0;
    Member* m = fileManager->loadAllMembers(count);
    delete[] m;
    return count;
}

int GymManager::getActiveMembers() {
    int count = 0;
    Member* m = getActiveMembersList(count);
    delete[] m;
    return count;
}

int GymManager::getTotalTrainers() {
    int count = 0;
    Trainer* t = fileManager->loadAllTrainers(count);
    delete[] t;
    return count;
}

double GymManager::getMonthlyRevenue() {
    QDate today = QDate::currentDate();
    return getTotalRevenueForMonth(today.month(), today.year());
}

int GymManager::getTodayCheckIns() {
    int count = 0;
    Attendance* a = getTodayAttendance(count);
    delete[] a;
    return count;
}