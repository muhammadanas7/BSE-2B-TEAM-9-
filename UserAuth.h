#ifndef USERAUTH_H
#define USERAUTH_H
#include <QString>
#include <QDate>
#include <QRegularExpression>

class UserAuth {
private:
    int     userId;
    QString username;
    QString passwordHash;
    QString email;
    QString role;
    QDate   lastLogin;
    bool    isActive;

public:
    UserAuth();
    UserAuth(int id, const QString& username, const QString& password,
        const QString& email, const QString& role);

    int     getUserId()    const;
    QString getUsername()  const;
    QString getEmail()     const;
    QString getRole()      const;
    QDate   getLastLogin() const;
    bool    getIsActive()  const;

    void setLastLogin(const QDate& date);
    void setIsActive(bool active);
    void setEmail(const QString& email);
    void setPassword(const QString& password);
    void setRole(const QString& role);

    bool verifyPassword(const QString& password) const;
    static bool isValidEmail(const QString& email);

    QString          toCSV() const;
    static UserAuth  fromCSV(const QString& line);
};

#endif