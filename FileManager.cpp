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
