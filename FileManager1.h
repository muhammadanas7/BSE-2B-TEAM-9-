#ifndef FILEMANAGERPART1_H
#define FILEMANAGERPART1_H

#include <QString>
#include <QStringList>
#include "Member.h"
#include "Trainer.h"

class FileManagerPart1 {
protected:
    QString dataDir;
    QString membersFile;
    QString trainersFile;
    
    bool ensureDataDir();
    bool writeLines(const QString& filePath, const QStringList& lines);
    QStringList readLines(const QString& filePath);

public:
    explicit FileManagerPart1(const QString& dataDirectory = "data");
    
    // Members
    bool saveMember(const Member& m);
    bool updateMember(const Member& m);
    bool deleteMember(int memberId);
    bool deleteAllMembers();
    Member loadMember(int memberId);
    Member* loadAllMembers(int& count);
    int getNextMemberId();
    
    // Trainers
    int getNextTrainerId();
    bool saveTrainer(const Trainer& t);
    bool updateTrainer(const Trainer& t);
    bool deleteTrainer(int trainerId);
    bool deleteAllTrainers();
    Trainer loadTrainer(int trainerId);
    Trainer* loadAllTrainers(int& count);
};

#endif
