#ifndef PAYMENT_H
#define PAYMENT_H
#include <QString>
#include <QDate>

class Payment {
private:
    int     paymentId;
    int     memberId;
    QString memberName;
    double  amount;
    QDate   paymentDate;
    QString paymentMethod;
    QString description;
    bool    isCleared;
    QString receiptNumber;

public:
    Payment();
    Payment(int pid, int mid, const QString& mName, double amount,
        const QDate& date, const QString& method,
        const QString& desc);

    int     getPaymentId()     const;
    int     getMemberId()      const;
    QString getMemberName()    const;
    double  getAmount()        const;
    QDate   getPaymentDate()   const;
    QString getPaymentMethod() const;
    QString getDescription()   const;
    bool    getIsCleared()     const;
    QString getReceiptNumber() const;

    void setAmount(double a);
    void setPaymentMethod(const QString& m);
    void setDescription(const QString& d);
    void setIsCleared(bool c);

    QString generateReceipt() const;
    QString getMonthYear()    const;

    QString        toCSV() const;
    static Payment fromCSV(const QString& line);
};

#endif