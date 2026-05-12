#include "UserAuth.h"
#include <QStringList>

UserAuth::UserAuth() : userId(0), isActive(true) {}

UserAuth::UserAuth(int id, const QString& uname, const QString& password,
    const QString& em, const QString& r)
    : userId(id), username(uname), passwordHash(password),
    email(em), role(r), isActive(true) {
}

int     UserAuth::getUserId()    const { return userId; }
QString UserAuth::getUsername()  const { return username; }
QString UserAuth::getEmail()     const { return email; }
QString UserAuth::getRole()      const { return role; }
QDate   UserAuth::getLastLogin() const { return lastLogin; }
bool    UserAuth::getIsActive()  const { return isActive; }

void UserAuth::setLastLogin(const QDate& date) { lastLogin = date; }
void UserAuth::setIsActive(bool active) { isActive = active; }
void UserAuth::setEmail(const QString& e) { email = e; }
void UserAuth::setPassword(const QString& password) { passwordHash = password; }
void UserAuth::setRole(const QString& r) { role = r; }

bool UserAuth::verifyPassword(const QString& password) const {
    return passwordHash == password;
}

bool UserAuth::isValidEmail(const QString& email) {
    QRegularExpression re(R"(^[a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,}$)");
    return re.match(email).hasMatch();
}

QString UserAuth::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(userId).arg(username).arg(passwordHash)
        .arg(email).arg(role)
        .arg(lastLogin.toString("yyyy-MM-dd"))
        .arg(isActive ? 1 : 0);
}

UserAuth UserAuth::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 7) return UserAuth();
    UserAuth u;
    u.userId = f[0].toInt();
    u.username = f[1];
    u.passwordHash = f[2];
    u.email = f[3];
    u.role = f[4];
    u.lastLogin = QDate::fromString(f[5], "yyyy-MM-dd");
    u.isActive = f[6].toInt() == 1;
    return u;
}