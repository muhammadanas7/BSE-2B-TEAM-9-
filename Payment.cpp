#include "Payment.h"
#include <QStringList>

Payment::Payment()
    : paymentId(0), memberId(0), amount(0.0), isCleared(false) {
}

Payment::Payment(int pid, int mid, const QString& mName, double amt,
    const QDate& date, const QString& method, const QString& desc)
    : paymentId(pid), memberId(mid), memberName(mName),
    amount(amt), paymentDate(date), paymentMethod(method),
    description(desc), isCleared(true) {
    receiptNumber = QString("REC-%1-%2").arg(pid).arg(date.toString("yyyyMMdd"));
}

int     Payment::getPaymentId()     const { return paymentId; }
int     Payment::getMemberId()      const { return memberId; }
QString Payment::getMemberName()    const { return memberName; }
double  Payment::getAmount()        const { return amount; }
QDate   Payment::getPaymentDate()   const { return paymentDate; }
QString Payment::getPaymentMethod() const { return paymentMethod; }
QString Payment::getDescription()   const { return description; }
bool    Payment::getIsCleared()     const { return isCleared; }
QString Payment::getReceiptNumber() const { return receiptNumber; }

void Payment::setAmount(double a) { amount = a; }
void Payment::setPaymentMethod(const QString& m) { paymentMethod = m; }
void Payment::setDescription(const QString& d) { description = d; }
void Payment::setIsCleared(bool c) { isCleared = c; }

QString Payment::generateReceipt() const {
    return QString(
        "==============================\n"
        "     GYM MANAGEMENT SYSTEM\n"
        "==============================\n"
        "Receipt No : %1\n"
        "Date       : %2\n"
        "Member     : %3 (ID: %4)\n"
        "Amount     : Rs. %5\n"
        "Method     : %6\n"
        "Details    : %7\n"
        "Status     : %8\n"
        "=============================="
    ).arg(receiptNumber)
        .arg(paymentDate.toString("dd-MMM-yyyy"))
        .arg(memberName).arg(memberId)
        .arg(amount, 0, 'f', 2)
        .arg(paymentMethod)
        .arg(description)
        .arg(isCleared ? "CLEARED" : "PENDING");
}

QString Payment::getMonthYear() const { return paymentDate.toString("MMM-yyyy"); }

QString Payment::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9")
        .arg(paymentId).arg(memberId).arg(memberName)
        .arg(amount)
        .arg(paymentDate.toString("yyyy-MM-dd"))
        .arg(paymentMethod)
        .arg(description)
        .arg(isCleared ? 1 : 0)
        .arg(receiptNumber);
}

Payment Payment::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 9) return Payment();
    Payment p;
    p.paymentId = f[0].toInt();
    p.memberId = f[1].toInt();
    p.memberName = f[2];
    p.amount = f[3].toDouble();
    p.paymentDate = QDate::fromString(f[4], "yyyy-MM-dd");
    p.paymentMethod = f[5];
    p.description = f[6];
    p.isCleared = f[7].toInt() == 1;
    p.receiptNumber = f[8];
    return p;
}