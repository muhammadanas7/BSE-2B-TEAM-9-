#ifndef MEMBERSHIPPLAN_H
#define MEMBERSHIPPLAN_H
#include <QString>

class MembershipPlan {
private:
    int     planId;
    QString planName;
    double  monthlyFee;
    int     durationMonths;
    QString features;
    bool    includesTrainer;
    bool    includesEquipment;

public:
    MembershipPlan();
    MembershipPlan(int id, const QString& name, double fee,
        int duration, const QString& features,
        bool trainer, bool equipment);

    int     getPlanId()            const;
    QString getPlanName()          const;
    double  getMonthlyFee()        const;
    int     getDurationMonths()    const;
    QString getFeatures()          const;
    bool    getIncludesTrainer()   const;
    bool    getIncludesEquipment() const;

    void setPlanName(const QString& n);
    void setMonthlyFee(double fee);
    void setDurationMonths(int d);
    void setFeatures(const QString& f);
    void setIncludesTrainer(bool b);
    void setIncludesEquipment(bool b);

    double  getTotalFee()                     const;
    double  getDiscountedFee(double pct)      const;
    QString getSummary()                      const;

    QString             toCSV() const;
    static MembershipPlan fromCSV(const QString& line);
};

#endif