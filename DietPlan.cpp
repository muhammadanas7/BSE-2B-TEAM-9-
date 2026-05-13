#include "DietPlan.h"
#include <QStringList>

DietPlan::DietPlan()
    : planId(0), calories(0), protein(0), carbs(0), fats(0) {
}

DietPlan::DietPlan(int id, const QString& name, const QString& desc,
    const QString& bf, const QString& lu,
    const QString& di, const QString& sn,
    int cal, double prot, double carb, double fat)
    : planId(id), planName(name), description(desc),
    breakfast(bf), lunch(lu), dinner(di), snacks(sn),
    calories(cal), protein(prot), carbs(carb), fats(fat) {
}

int     DietPlan::getPlanId()      const { return planId; }
QString DietPlan::getPlanName()    const { return planName; }
QString DietPlan::getDescription() const { return description; }
QString DietPlan::getBreakfast()   const { return breakfast; }
QString DietPlan::getLunch()       const { return lunch; }
QString DietPlan::getDinner()      const { return dinner; }
QString DietPlan::getSnacks()      const { return snacks; }
int     DietPlan::getCalories()    const { return calories; }
double  DietPlan::getProtein()     const { return protein; }
double  DietPlan::getCarbs()       const { return carbs; }
double  DietPlan::getFats()        const { return fats; }

QString DietPlan::getFullPlan() const {
    return QString(
        "============================================\n"
        "            %1 DIET PLAN\n"
        "============================================\n\n"
        "DESCRIPTION:\n  %2\n\n"
        "NUTRITION FACTS:\n"
        "  Calories : %3 kcal\n"
        "  Protein  : %4 g\n"
        "  Carbs    : %5 g\n"
        "  Fats     : %6 g\n\n"
        "MEAL PLAN:\n"
        "  Breakfast : %7\n"
        "  Lunch     : %8\n"
        "  Dinner    : %9\n"
        "  Snacks    : %10\n"
        "============================================"
    ).arg(planName.toUpper()).arg(description)
        .arg(calories)
        .arg(protein, 0, 'f', 1)
        .arg(carbs, 0, 'f', 1)
        .arg(fats, 0, 'f', 1)
        .arg(breakfast).arg(lunch).arg(dinner).arg(snacks);
}

QString DietPlan::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
        .arg(planId).arg(planName).arg(description)
        .arg(breakfast).arg(lunch).arg(dinner).arg(snacks)
        .arg(calories).arg(protein).arg(carbs).arg(fats);
}

DietPlan DietPlan::fromCSV(const QString& line) {
    QStringList f = line.split(',');
    if (f.size() < 11) return DietPlan();
    DietPlan d;
    d.planId = f[0].toInt();
    d.planName = f[1];
    d.description = f[2];
    d.breakfast = f[3];
    d.lunch = f[4];
    d.dinner = f[5];
    d.snacks = f[6];
    d.calories = f[7].toInt();
    d.protein = f[8].toDouble();
    d.carbs = f[9].toDouble();
    d.fats = f[10].toDouble();
    return d;
}