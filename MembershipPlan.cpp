#include "MembershipPlan.h"
#include <QStringList>

MembershipPlan::MembershipPlan()
    : planId(0), monthlyFee(0.0), durationMonths(1),
    includesTrainer(false), includesEquipment(false) {
}

MembershipPlan::MembershipPlan(int id, const QString& n, double fee,
    int dur, const QString& feat,
    bool trainer, bool equip)
    : planId(id), planName(n), monthlyFee(fee), durationMonths(dur),
    features(feat), includesTrainer(trainer), includesEquipment(equip) {
}

int     MembershipPlan::getPlanId()            const { return planId; }
QString MembershipPlan::getPlanName()          const { return planName; }
double  MembershipPlan::getMonthlyFee()        const { return monthlyFee; }
int     MembershipPlan::getDurationMonths()    const { return durationMonths; }
QString MembershipPlan::getFeatures()          const { return features; }
bool    MembershipPlan::getIncludesTrainer()   const { return includesTrainer; }
bool    MembershipPlan::getIncludesEquipment() const { return includesEquipment; }

void MembershipPlan::setPlanName(const QString& n) { planName = n; }
void MembershipPlan::setMonthlyFee(double f) { monthlyFee = f; }
void MembershipPlan::setDurationMonths(int d) { durationMonths = d; }
void MembershipPlan::setFeatures(const QString& f) { features = f; }
void MembershipPlan::setIncludesTrainer(bool b) { includesTrainer = b; }
void MembershipPlan::setIncludesEquipment(bool b) { includesEquipment = b; }

double MembershipPlan::getTotalFee() const { return monthlyFee * durationMonths; }

double MembershipPlan::getDiscountedFee(double pct) const {
    return getTotalFee() * (1.0 - pct / 100.0);
}

QString MembershipPlan::getSummary() const {
    return QString("%1 – Rs.%2/mo | %3 months | Trainer: %4 | Equipment: %5")
        .arg(planName).arg(monthlyFee).arg(durationMonths)
        .arg(includesTrainer ? "Yes" : "No")
        .arg(includesEquipment ? "Yes" : "No");
}

QString MembershipPlan::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(planId).arg(planName).arg(monthlyFee)
        .arg(durationMonths).arg(features)
        .arg(includesTrainer ? 1 : 0)
        .arg(includesEquipment ? 1 : 0);
}

MembershipPlan MembershipPlan::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 7) return MembershipPlan();
    MembershipPlan p;
    p.planId = f[0].toInt();
    p.planName = f[1];
    p.monthlyFee = f[2].toDouble();
    p.durationMonths = f[3].toInt();
    p.features = f[4];
    p.includesTrainer = f[5].toInt() == 1;
    p.includesEquipment = f[6].toInt() == 1;
    return p;
}