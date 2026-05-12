#include "FileManager.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDate>

FileManager::FileManager(const QString& dataDirectory) : dataDir(dataDirectory) {
    membersFile = dataDir + "/members.csv";
    trainersFile = dataDir + "/trainers.csv";
    plansFile = dataDir + "/plans.csv";
    attendanceFile = dataDir + "/attendance.csv";
    equipmentFile = dataDir + "/equipment.csv";
    paymentsFile = dataDir + "/payments.csv";
    usersFile = dataDir + "/users.csv";
    workoutsFile = dataDir + "/workouts.csv";
    sessionsFile = dataDir + "/sessions.csv";
    dietPlansFile = dataDir + "/dietplans.csv";
    ensureDataDir();
}

bool FileManager::ensureDataDir() {
    QDir dir;
    if (!dir.exists(dataDir)) return dir.mkpath(dataDir);
    return true;
}

bool FileManager::writeLines(const QString& filePath, const QStringList& lines) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    for (const QString& line : lines) {
        if (!line.trimmed().isEmpty()) {
            out << line << "\n";
        }
    }
    file.close();
    return true;
}

QStringList FileManager::readLines(const QString& filePath) {
    QStringList lines;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            lines.append(line);
        }
    }
    file.close();
    return lines;
}

// ── Members ──────────────────────────────────────────────────
int FileManager::getNextMemberId() {
    QStringList lines = readLines(membersFile);
    int maxId = 0;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        int id = line.split(',').first().toInt();
        if (id > maxId) maxId = id;
    }
    return maxId + 1;
}

bool FileManager::saveMember(const Member& m) {
    if (m.getMemberId() <= 0) return false;
    QFile file(membersFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << m.toCSV() << "\n";
    file.close();
    return true;
}

bool FileManager::updateMember(const Member& m) {
    if (m.getMemberId() <= 0) return false;
    QStringList lines = readLines(membersFile);
    QStringList updated;
    bool found = false;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() == m.getMemberId()) {
            updated.append(m.toCSV());
            found = true;
        }
        else {
            updated.append(line);
        }
    }
    if (!found) return false;
    return writeLines(membersFile, updated);
}

bool FileManager::deleteMember(int memberId) {
    if (memberId <= 0) return false;
    QStringList lines = readLines(membersFile);
    QStringList updated;
    bool found = false;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() != memberId) {
            updated.append(line);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    return writeLines(membersFile, updated);
}

bool FileManager::deleteAllMembers() {
    QFile file(membersFile);
    if (file.exists()) {
        file.remove();
    }
    // Create fresh empty file
    QFile newFile(membersFile);
    if (newFile.open(QIODevice::WriteOnly)) {
        newFile.close();
    }
    return true;
}

Member FileManager::loadMember(int memberId) {
    if (memberId <= 0) return Member();
    QStringList lines = readLines(membersFile);
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() == memberId) {
            return Member::fromCSV(line);
        }
    }
    return Member();
}

Member* FileManager::loadAllMembers(int& count) {
    QStringList lines = readLines(membersFile);
    QList<Member> validMembers;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        Member m = Member::fromCSV(line);
        if (m.getMemberId() > 0 && !m.getName().isEmpty()) {
            validMembers.append(m);
        }
    }
    count = validMembers.size();
    if (count == 0) return nullptr;
    Member* arr = new Member[count];
    for (int i = 0; i < count; ++i) {
        arr[i] = validMembers[i];
    }
    return arr;
}

// ── Trainers ─────────────────────────────────────────────────
int FileManager::getNextTrainerId() {
    QStringList lines = readLines(trainersFile);
    int maxId = 0;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        int id = line.split(',').first().toInt();
        if (id > maxId) maxId = id;
    }
    return maxId + 1;
}

bool FileManager::saveTrainer(const Trainer& t) {
    if (t.getTrainerId() <= 0) return false;
    QFile file(trainersFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << t.toCSV() << "\n";
    file.close();
    return true;
}

bool FileManager::updateTrainer(const Trainer& t) {
    if (t.getTrainerId() <= 0) return false;
    QStringList lines = readLines(trainersFile);
    QStringList updated;
    bool found = false;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() == t.getTrainerId()) {
            updated.append(t.toCSV());
            found = true;
        }
        else {
            updated.append(line);
        }
    }
    if (!found) return false;
    return writeLines(trainersFile, updated);
}

bool FileManager::deleteTrainer(int trainerId) {
    if (trainerId <= 0) return false;
    QStringList lines = readLines(trainersFile);
    QStringList updated;
    bool found = false;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() != trainerId) {
            updated.append(line);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    return writeLines(trainersFile, updated);
}

bool FileManager::deleteAllTrainers() {
    QFile file(trainersFile);
    if (file.exists()) {
        file.remove();
        QFile newFile(trainersFile);
        newFile.open(QIODevice::WriteOnly);
        newFile.close();
    }
    return true;
}

Trainer FileManager::loadTrainer(int trainerId) {
    if (trainerId <= 0) return Trainer();
    QStringList lines = readLines(trainersFile);
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() == trainerId)
            return Trainer::fromCSV(line);
    }
    return Trainer();
}

Trainer* FileManager::loadAllTrainers(int& count) {
    QStringList lines = readLines(trainersFile);
    QList<Trainer> validTrainers;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        Trainer t = Trainer::fromCSV(line);
        if (t.getTrainerId() > 0 && !t.getName().isEmpty()) {
            validTrainers.append(t);
        }
    }
    count = validTrainers.size();
    if (count == 0) return nullptr;
    Trainer* arr = new Trainer[count];
    for (int i = 0; i < count; ++i) arr[i] = validTrainers[i];
    return arr;
} 



// ── Payments ─────────────────────────────────────────────────
int FileManager::getNextPaymentId() {
    QStringList lines = readLines(paymentsFile);
    int maxId = 0;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        int id = line.split(',').first().toInt();
        if (id > maxId) maxId = id;
    }
    return maxId + 1;
}

bool FileManager::savePayment(const Payment& p) {
    QFile file(paymentsFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << p.toCSV() << "\n";
    file.close();
    return true;
}

bool FileManager::deletePaymentsForMember(int memberId) {
    QStringList lines = readLines(paymentsFile);
    QStringList updated;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').at(1).toInt() != memberId) updated.append(line);
    }
    return writeLines(paymentsFile, updated);
}

bool FileManager::deleteAllPayments() {
    QFile file(paymentsFile);
    if (file.exists()) return file.remove();
    return true;
}

Payment* FileManager::loadAllPayments(int& count) {
    QStringList lines = readLines(paymentsFile);
    QList<Payment> validPayments;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        validPayments.append(Payment::fromCSV(line));
    }
    count = validPayments.size();
    if (count == 0) return nullptr;
    Payment* arr = new Payment[count];
    for (int i = 0; i < count; ++i) arr[i] = validPayments[i];
    return arr;
}

Payment* FileManager::loadPaymentsForMember(int memberId, int& count) {
    QStringList lines = readLines(paymentsFile);
    QStringList filtered;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').at(1).toInt() == memberId) filtered.append(line);
    }
    count = filtered.size();
    if (count == 0) return nullptr;
    Payment* arr = new Payment[count];
    for (int i = 0; i < count; ++i) arr[i] = Payment::fromCSV(filtered[i]);
    return arr;
}

// ── Users ────────────────────────────────────────────────────
int FileManager::getNextUserId() {
    QStringList lines = readLines(usersFile);
    int maxId = 0;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        int id = line.split(',').first().toInt();
        if (id > maxId) maxId = id;
    }
    return maxId + 1;
}

bool FileManager::saveUser(const UserAuth& u) {
    QFile file(usersFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << u.toCSV() << "\n";
    file.close();
    return true;
}

bool FileManager::updateUser(const UserAuth& u) {
    QStringList lines = readLines(usersFile);
    QStringList updated;
    bool found = false;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() == u.getUserId()) {
            updated.append(u.toCSV());
            found = true;
        }
        else {
            updated.append(line);
        }
    }
    if (!found) return false;
    return writeLines(usersFile, updated);
}

bool FileManager::deleteUser(int userId) {
    QStringList lines = readLines(usersFile);
    QStringList updated;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() != userId) updated.append(line);
    }
    return writeLines(usersFile, updated);
}

UserAuth FileManager::loadUser(const QString& username) {
    QStringList lines = readLines(usersFile);
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        UserAuth u = UserAuth::fromCSV(line);
        if (u.getUsername().toLower() == username.toLower()) return u;
    }
    return UserAuth();
}

UserAuth* FileManager::loadAllUsers(int& count) {
    QStringList lines = readLines(usersFile);
    QList<UserAuth> validUsers;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        validUsers.append(UserAuth::fromCSV(line));
    }
    count = validUsers.size();
    if (count == 0) return nullptr;
    UserAuth* arr = new UserAuth[count];
    for (int i = 0; i < count; ++i) arr[i] = validUsers[i];
    return arr;
}

bool FileManager::userExists(const QString& username) {
    int count = 0;
    UserAuth* users = loadAllUsers(count);
    bool exists = false;
    for (int i = 0; i < count; ++i)
        if (users[i].getUsername().toLower() == username.toLower()) { exists = true; break; }
    delete[] users;
    return exists;
}

bool FileManager::emailExists(const QString& email) {
    int count = 0;
    UserAuth* users = loadAllUsers(count);
    bool exists = false;
    for (int i = 0; i < count; ++i)
        if (users[i].getEmail().toLower() == email.toLower()) { exists = true; break; }
    delete[] users;
    return exists;
}

// ── Workouts ─────────────────────────────────────────────────
int FileManager::getNextWorkoutId() {
    QStringList lines = readLines(workoutsFile);
    int maxId = 0;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        int id = line.split(',').first().toInt();
        if (id > maxId) maxId = id;
    }
    return maxId + 1;
}

bool FileManager::saveWorkout(const Workout& w) {
    QFile file(workoutsFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << w.toCSV() << "\n";
    file.close();
    return true;
}

bool FileManager::updateWorkout(const Workout& w) {
    QStringList lines = readLines(workoutsFile);
    QStringList updated;
    bool found = false;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').first().toInt() == w.getWorkoutId()) {
            updated.append(w.toCSV());
            found = true;
        }
        else {
            updated.append(line);
        }
    }
    if (!found) return false;
    return writeLines(workoutsFile, updated);
}

bool FileManager::deleteWorkoutsForMember(int memberId) {
    QStringList lines = readLines(workoutsFile);
    QStringList updated;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').at(1).toInt() != memberId) updated.append(line);
    }
    return writeLines(workoutsFile, updated);
}

bool FileManager::deleteAllWorkouts() {
    QFile file(workoutsFile);
    if (file.exists()) return file.remove();
    return true;
}

Workout* FileManager::loadWorkoutsForMember(int memberId, int& count) {
    QStringList lines = readLines(workoutsFile);
    QStringList filtered;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        if (line.split(',').at(1).toInt() == memberId) filtered.append(line);
    }
    count = filtered.size();
    if (count == 0) return nullptr;
    Workout* arr = new Workout[count];
    for (int i = 0; i < count; ++i) arr[i] = Workout::fromCSV(filtered[i]);
    return arr;
}

Workout* FileManager::loadAllWorkouts(int& count) {
    QStringList lines = readLines(workoutsFile);
    QList<Workout> validWorkouts;
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;
        validWorkouts.append(Workout::fromCSV(line));
    }
    count = validWorkouts.size();
    if (count == 0) return nullptr;
    Workout* arr = new Workout[count];
    for (int i = 0; i < count; ++i) arr[i] = validWorkouts[i];
    return arr;


    bool FileManager::savePlan(const MembershipPlan & p) {
        QFile file(plansFile);
        if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
        QTextStream out(&file);
        out << p.toCSV() << "\n";
        file.close();
        return true;
    }

    bool FileManager::updatePlan(const MembershipPlan & p) {
        QStringList lines = readLines(plansFile);
        QStringList updated;
        bool found = false;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            if (line.split(',').first().toInt() == p.getPlanId()) {
                updated.append(p.toCSV());
                found = true;
            }
            else {
                updated.append(line);
            }
        }
        if (!found) return false;
        return writeLines(plansFile, updated);
    }

    bool FileManager::deletePlan(int planId) {
        QStringList lines = readLines(plansFile);
        QStringList updated;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            if (line.split(',').first().toInt() != planId) updated.append(line);
        }
        return writeLines(plansFile, updated);
    }

    MembershipPlan* FileManager::loadAllPlans(int& count) {
        QStringList lines = readLines(plansFile);
        QList<MembershipPlan> validPlans;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            validPlans.append(MembershipPlan::fromCSV(line));
        }
        count = validPlans.size();
        if (count == 0) return nullptr;
        MembershipPlan* arr = new MembershipPlan[count];
        for (int i = 0; i < count; ++i) arr[i] = validPlans[i];
        return arr;
    }

    // ── Attendance ───────────────────────────────────────────────
    int FileManager::getNextAttendanceId() {
        QStringList lines = readLines(attendanceFile);
        int maxId = 0;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            int id = line.split(',').first().toInt();
            if (id > maxId) maxId = id;
        }
        return maxId + 1;
    }

    bool FileManager::saveAttendance(const Attendance & a) {
        QFile file(attendanceFile);
        if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
        QTextStream out(&file);
        out << a.toCSV() << "\n";
        file.close();
        return true;
    }

    bool FileManager::deleteAttendanceForMember(int memberId) {
        QStringList lines = readLines(attendanceFile);
        QStringList updated;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            if (line.split(',').at(1).toInt() != memberId) updated.append(line);
        }
        return writeLines(attendanceFile, updated);
    }

    bool FileManager::deleteAllAttendance() {
        QFile file(attendanceFile);
        if (file.exists()) return file.remove();
        return true;
    }

    Attendance* FileManager::loadAttendanceForMember(int memberId, int& count) {
        QStringList lines = readLines(attendanceFile);
        QStringList filtered;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            if (line.split(',').at(1).toInt() == memberId) filtered.append(line);
        }
        count = filtered.size();
        if (count == 0) return nullptr;
        Attendance* arr = new Attendance[count];
        for (int i = 0; i < count; ++i) arr[i] = Attendance::fromCSV(filtered[i]);
        return arr;
    }

    Attendance* FileManager::loadAllAttendance(int& count) {
        QStringList lines = readLines(attendanceFile);
        QList<Attendance> validAttendance;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            validAttendance.append(Attendance::fromCSV(line));
        }
        count = validAttendance.size();
        if (count == 0) return nullptr;
        Attendance* arr = new Attendance[count];
        for (int i = 0; i < count; ++i) arr[i] = validAttendance[i];
        return arr;
    }

    // ── Equipment ────────────────────────────────────────────────
    int FileManager::getNextEquipmentId() {
        QStringList lines = readLines(equipmentFile);
        int maxId = 0;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            int id = line.split(',').first().toInt();
            if (id > maxId) maxId = id;
        }
        return maxId + 1;
    }

    bool FileManager::saveEquipment(const Equipment & e) {
        QFile file(equipmentFile);
        if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
        QTextStream out(&file);
        out << e.toCSV() << "\n";
        file.close();
        return true;
    }

    bool FileManager::updateEquipment(const Equipment & e) {
        QStringList lines = readLines(equipmentFile);
        QStringList updated;
        bool found = false;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            if (line.split(',').first().toInt() == e.getEquipmentId()) {
                updated.append(e.toCSV());
                found = true;
            }
            else {
                updated.append(line);
            }
        }
        if (!found) return false;
        return writeLines(equipmentFile, updated);
    }

    bool FileManager::deleteEquipment(int equipId) {
        QStringList lines = readLines(equipmentFile);
        QStringList updated;
        bool found = false;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            if (line.split(',').first().toInt() != equipId) {
                updated.append(line);
            }
            else {
                found = true;
            }
        }
        if (!found) return false;
        return writeLines(equipmentFile, updated);
    }

    bool FileManager::deleteAllEquipment() {
        QFile file(equipmentFile);
        if (file.exists()) return file.remove();
        return true;
    }

    Equipment* FileManager::loadAllEquipment(int& count) {
        QStringList lines = readLines(equipmentFile);
        QList<Equipment> validEquipment;
        for (const QString& line : lines) {
            if (line.trimmed().isEmpty()) continue;
            validEquipment.append(Equipment::fromCSV(line));
        }
        count = validEquipment.size();
        if (count == 0) return nullptr;
        Equipment* arr = new Equipment[count];
        for (int i = 0; i < count; ++i) arr[i] = validEquipment[i];
        return arr;
    }
