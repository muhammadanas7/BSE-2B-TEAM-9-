#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include <QString>
#include <QDate>

class Equipment {
private:
    int     equipmentId;
    QString name;
    QString category;
    QString brand;
    int     quantity;
    int     availableQty;
    QDate   purchaseDate;
    double  purchasePrice;
    QDate   lastMaintenanceDate;
    QDate   nextMaintenanceDate;
    QString condition;

public:
    Equipment();
    Equipment(int id, const QString& name, const QString& category,
        const QString& brand, int qty, double price,
        const QDate& purchaseDate);

    int     getEquipmentId()         const;
    QString getName()                const;
    QString getCategory()            const;
    QString getBrand()               const;
    int     getQuantity()            const;
    int     getAvailableQty()        const;
    QDate   getPurchaseDate()        const;
    double  getPurchasePrice()       const;
    QDate   getLastMaintenanceDate() const;
    QDate   getNextMaintenanceDate() const;
    QString getCondition()           const;

    void setName(const QString& n);
    void setCategory(const QString& c);
    void setQuantity(int q);
    void setAvailableQty(int q);
    void setCondition(const QString& c);
    void setLastMaintenanceDate(const QDate& d);
    void setNextMaintenanceDate(const QDate& d);

    bool    isMaintenanceDue() const;
    bool    isAvailable()      const;
    void    checkout();
    void    checkin();
    QString getStatusString()  const;

    QString         toCSV() const;
    static Equipment fromCSV(const QString& line);
};

#endif