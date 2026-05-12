#ifndef MEMBER_H
#define MEMBER_H
#include <QString>
#include <QDate>
#include <QRegularExpression>

class Member {
private:
    int     memberId;
    QString name;
    QString cnic;
    QString phone;
    QString email;
    QString address;
    QString gender;
    QDate   dateOfBirth;
    QDate   joinDate;
    QString membershipPlan;
    bool    isActive;
    double  totalFeesPaid;
    double  pendingFees;
    bool    wantsTrainer;
    QString dietPlan;
    int     assignedTrainerId;

public:
    Member();
    Member(int id, const QString& name, const QString& cnic,
        const QString& phone, const QString& email,
        const QString& address, const QString& gender,
        const QDate& dob, const QString& plan,
        bool wantsTrainer = false,
        const QString& dietPlan = "None",
        int assignedTrainerId = -1);

    int     getMemberId()        const;
    QString getName()            const;
    QString getCnic()            const;
    QString getPhone()           const;
    QString getEmail()           const;
    QString getAddress()         const;
    QString getGender()          const;
    QDate   getDateOfBirth()     const;
    QDate   getJoinDate()        const;
    QString getMembershipPlan()  const;
    bool    getIsActive()        const;
    double  getTotalFeesPaid()   const;
    double  getPendingFees()     const;
    bool    getWantsTrainer()    const;
    QString getDietPlan()        const;
    int     getAssignedTrainerId() const;

    void setName(const QString& n);
    void setPhone(const QString& p);
    void setEmail(const QString& e);
    void setAddress(const QString& a);
    void setMembershipPlan(const QString& plan);
    void setIsActive(bool active);
    void setTotalFeesPaid(double amount);
    void setPendingFees(double amount);
    void setCnic(const QString& c);
    void setWantsTrainer(bool wants);
    void setDietPlan(const QString& plan);
    void setAssignedTrainerId(int id);

    static double  getPlanFee(const QString& plan);
    static double  getTrainerExtraFee();
    static bool    isValidCnic(const QString& cnic);
    static bool    isValidPhone(const QString& phone);
    static bool    isValidEmail(const QString& email);
    static QString getCnicValidationMessage();
    static QString getPhoneValidationMessage();

    void    addPayment(double amount);
    void    addPendingFee(double amount);
    bool    isMembershipExpired() const;
    int     getAge()              const;
    QString getStatusString()     const;
    double  getTotalDue()         const;

    QString       toCSV() const;
    static Member fromCSV(const QString& line);
};

#endif