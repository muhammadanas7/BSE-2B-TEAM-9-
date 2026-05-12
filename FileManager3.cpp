
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
