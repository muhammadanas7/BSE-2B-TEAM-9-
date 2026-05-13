#ifndef DIETPLAN_H
#define DIETPLAN_H
#include <QString>

class DietPlan {
private:
    int     planId;
    QString planName;
    QString description;
    QString breakfast;
    QString lunch;
    QString dinner;
    QString snacks;
    int     calories;
    double  protein;
    double  carbs;
    double  fats;

public:
    DietPlan();
    DietPlan(int id, const QString& name, const QString& desc,
        const QString& breakfast, const QString& lunch,
        const QString& dinner, const QString& snacks,
        int calories, double protein, double carbs, double fats);

    int     getPlanId()      const;
    QString getPlanName()    const;
    QString getDescription() const;
    QString getBreakfast()   const;
    QString getLunch()       const;
    QString getDinner()      const;
    QString getSnacks()      const;
    int     getCalories()    const;
    double  getProtein()     const;
    double  getCarbs()       const;
    double  getFats()        const;

    QString getFullPlan() const;

    QString        toCSV() const;
    static DietPlan fromCSV(const QString& line);
};

#endif