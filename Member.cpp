#include "Member.h"
#include <QStringList>

Member::Member()
    : memberId(0), isActive(false), totalFeesPaid(0.0), pendingFees(0.0),
    wantsTrainer(false), dietPlan("None"), assignedTrainerId(-1) {
    joinDate = QDate::currentDate();
}

Member::Member(int id, const QString& n, const QString& cnic,
    const QString& phone, const QString& email,
    const QString& address, const QString& gender,
    const QDate& dob, const QString& plan,
    bool wantsTrainer, const QString& dietPlan,
    int assignedTrainerId)
    : memberId(id), name(n), cnic(cnic), phone(phone),
    email(email), address(address), gender(gender),
    dateOfBirth(dob), membershipPlan(plan),
    isActive(true), totalFeesPaid(0.0), pendingFees(0.0),
    wantsTrainer(wantsTrainer), dietPlan(dietPlan),
    assignedTrainerId(assignedTrainerId) {
    joinDate = QDate::currentDate();
}

int     Member::getMemberId()        const { return memberId; }
QString Member::getName()            const { return name; }
QString Member::getCnic()            const { return cnic; }
QString Member::getPhone()           const { return phone; }
QString Member::getEmail()           const { return email; }
QString Member::getAddress()         const { return address; }
QString Member::getGender()          const { return gender; }
QDate   Member::getDateOfBirth()     const { return dateOfBirth; }
QDate   Member::getJoinDate()        const { return joinDate; }
QString Member::getMembershipPlan()  const { return membershipPlan; }
bool    Member::getIsActive()        const { return isActive; }
double  Member::getTotalFeesPaid()   const { return totalFeesPaid; }
double  Member::getPendingFees()     const { return pendingFees; }
bool    Member::getWantsTrainer()    const { return wantsTrainer; }
QString Member::getDietPlan()        const { return dietPlan; }
int     Member::getAssignedTrainerId() const { return assignedTrainerId; }

void Member::setName(const QString& n) { name = n; }
void Member::setPhone(const QString& p) { phone = p; }
void Member::setEmail(const QString& e) { email = e; }
void Member::setAddress(const QString& a) { address = a; }
void Member::setMembershipPlan(const QString& p) { membershipPlan = p; }
void Member::setIsActive(bool a) { isActive = a; }
void Member::setTotalFeesPaid(double amt) { totalFeesPaid = amt; }
void Member::setPendingFees(double amt) { pendingFees = amt; }
void Member::setCnic(const QString& c) { cnic = c; }
void Member::setWantsTrainer(bool wants) { wantsTrainer = wants; }
void Member::setDietPlan(const QString& plan) { dietPlan = plan; }
void Member::setAssignedTrainerId(int id) { assignedTrainerId = id; }

double Member::getPlanFee(const QString& plan) {
    if (plan == "Basic")    return 3000.0;
    if (plan == "Standard") return 5000.0;
    if (plan == "Premium")  return 8000.0;
    return 0.0;
}

double Member::getTrainerExtraFee() { return 1000.0; }

bool Member::isValidCnic(const QString& cnic) {
    QRegularExpression re(R"(^\d{13}$)");
    return re.match(cnic).hasMatch();
}

bool Member::isValidPhone(const QString& phone) {
    QRegularExpression re(R"(^\d{11}$)");
    return re.match(phone).hasMatch();
}

bool Member::isValidEmail(const QString& email) {
    if (email.isEmpty()) return true;
    QRegularExpression re(R"(^[a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,}$)");
    return re.match(email).hasMatch();
}

QString Member::getCnicValidationMessage() { return "CNIC must be exactly 13 digits (e.g. 1234567890123)"; }
QString Member::getPhoneValidationMessage() { return "Phone must be exactly 11 digits (e.g. 03123456789)"; }

void Member::addPayment(double amount) {
    totalFeesPaid += amount;
    pendingFees = (pendingFees >= amount) ? pendingFees - amount : 0.0;
}

void Member::addPendingFee(double amount) { pendingFees += amount; }
bool Member::isMembershipExpired() const { return joinDate.addDays(30) < QDate::currentDate(); }
int  Member::getAge() const { return dateOfBirth.daysTo(QDate::currentDate()) / 365; }
QString Member::getStatusString() const { return isActive ? "Active" : "Inactive"; }

double Member::getTotalDue() const {
    double fee = getPlanFee(membershipPlan);
    if (wantsTrainer && (membershipPlan == "Standard" || membershipPlan == "Premium"))
        fee += getTrainerExtraFee();
    return fee;
}

QString Member::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16")
        .arg(memberId).arg(name).arg(cnic).arg(phone).arg(email)
        .arg(address).arg(gender)
        .arg(dateOfBirth.toString("yyyy-MM-dd"))
        .arg(joinDate.toString("yyyy-MM-dd"))
        .arg(membershipPlan)
        .arg(isActive ? 1 : 0)
        .arg(totalFeesPaid)
        .arg(pendingFees)
        .arg(wantsTrainer ? 1 : 0)
        .arg(dietPlan)
        .arg(assignedTrainerId);
}

Member Member::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 16) return Member();
    Member m;
    m.memberId = f[0].toInt();
    m.name = f[1];
    m.cnic = f[2];
    m.phone = f[3];
    m.email = f[4];
    m.address = f[5];
    m.gender = f[6];
    m.dateOfBirth = QDate::fromString(f[7], "yyyy-MM-dd");
    m.joinDate = QDate::fromString(f[8], "yyyy-MM-dd");
    m.membershipPlan = f[9];
    m.isActive = f[10].toInt() == 1;
    m.totalFeesPaid = f[11].toDouble();
    m.pendingFees = f[12].toDouble();
    m.wantsTrainer = f[13].toInt() == 1;
    m.dietPlan = f[14];
    m.assignedTrainerId = f[15].toInt();
    return m;
}