#include "Equipment.h"
#include <QStringList>

Equipment::Equipment()
    : equipmentId(0), quantity(0), availableQty(0), purchasePrice(0.0) {
}

Equipment::Equipment(int id, const QString& n, const QString& cat,
    const QString& br, int qty, double price, const QDate& pDate)
    : equipmentId(id), name(n), category(cat), brand(br),
    quantity(qty), availableQty(qty), purchaseDate(pDate),
    purchasePrice(price), condition("Excellent") {
    lastMaintenanceDate = pDate;
    nextMaintenanceDate = pDate.addMonths(3);
}

int     Equipment::getEquipmentId()         const { return equipmentId; }
QString Equipment::getName()                const { return name; }
QString Equipment::getCategory()            const { return category; }
QString Equipment::getBrand()               const { return brand; }
int     Equipment::getQuantity()            const { return quantity; }
int     Equipment::getAvailableQty()        const { return availableQty; }
QDate   Equipment::getPurchaseDate()        const { return purchaseDate; }
double  Equipment::getPurchasePrice()       const { return purchasePrice; }
QDate   Equipment::getLastMaintenanceDate() const { return lastMaintenanceDate; }
QDate   Equipment::getNextMaintenanceDate() const { return nextMaintenanceDate; }
QString Equipment::getCondition()           const { return condition; }

void Equipment::setName(const QString& n) { name = n; }
void Equipment::setCategory(const QString& c) { category = c; }
void Equipment::setQuantity(int q) { quantity = q; }
void Equipment::setAvailableQty(int q) { availableQty = q; }
void Equipment::setCondition(const QString& c) { condition = c; }
void Equipment::setLastMaintenanceDate(const QDate& d) { lastMaintenanceDate = d; }
void Equipment::setNextMaintenanceDate(const QDate& d) { nextMaintenanceDate = d; }

bool Equipment::isMaintenanceDue() const { return QDate::currentDate() >= nextMaintenanceDate; }
bool Equipment::isAvailable()      const { return availableQty > 0; }
void Equipment::checkout() { if (availableQty > 0) --availableQty; }
void Equipment::checkin() { if (availableQty < quantity) ++availableQty; }

QString Equipment::getStatusString() const {
    if (isMaintenanceDue()) return "Maintenance Due";
    if (!isAvailable())     return "All Checked Out";
    return "Available";
}

QString Equipment::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
        .arg(equipmentId).arg(name).arg(category).arg(brand)
        .arg(quantity).arg(availableQty)
        .arg(purchaseDate.toString("yyyy-MM-dd"))
        .arg(purchasePrice)
        .arg(lastMaintenanceDate.toString("yyyy-MM-dd"))
        .arg(nextMaintenanceDate.toString("yyyy-MM-dd"))
        .arg(condition);
}

Equipment Equipment::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 11) return Equipment();
    Equipment e;
    e.equipmentId = f[0].toInt();
    e.name = f[1];
    e.category = f[2];
    e.brand = f[3];
    e.quantity = f[4].toInt();
    e.availableQty = f[5].toInt();
    e.purchaseDate = QDate::fromString(f[6], "yyyy-MM-dd");
    e.purchasePrice = f[7].toDouble();
    e.lastMaintenanceDate = QDate::fromString(f[8], "yyyy-MM-dd");
    e.nextMaintenanceDate = QDate::fromString(f[9], "yyyy-MM-dd");
    e.condition = f[10];
    return e;
}