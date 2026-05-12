

void MainWindow::populatePaymentsTable() {
    paymentsTable->setRowCount(0);
    int count = 0;
    Payment* payments = gymManager->getAllPayments(count);
    if (!payments) return;
    for (int i = 0; i < count; ++i) {
        int row = paymentsTable->rowCount();
        paymentsTable->insertRow(row);
        paymentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(payments[i].getPaymentId())));
        paymentsTable->setItem(row, 1, new QTableWidgetItem(payments[i].getMemberName()));
        paymentsTable->setItem(row, 2, new QTableWidgetItem("Rs. " + QString::number(payments[i].getAmount(), 'f', 0)));
        paymentsTable->setItem(row, 3, new QTableWidgetItem(payments[i].getPaymentDate().toString("dd-MMM-yyyy")));
        paymentsTable->setItem(row, 4, new QTableWidgetItem(payments[i].getPaymentMethod()));
        paymentsTable->setItem(row, 5, new QTableWidgetItem(payments[i].getDescription()));
        paymentsTable->setItem(row, 6, new QTableWidgetItem(payments[i].getIsCleared() ? "Cleared" : "Pending"));
    }
    delete[] payments;
}

void MainWindow::populateWorkoutsTable() {
    workoutsTable->setRowCount(0);
    int count = 0;
    Workout* workouts = gymManager->getAllWorkouts(count);
    if (!workouts) return;
    for (int i = 0; i < count; ++i) {
        int row = workoutsTable->rowCount();
        workoutsTable->insertRow(row);
        workoutsTable->setItem(row, 0, new QTableWidgetItem(QString::number(workouts[i].getWorkoutId())));
        workoutsTable->setItem(row, 1, new QTableWidgetItem(workouts[i].getMemberName()));
        workoutsTable->setItem(row, 2, new QTableWidgetItem(workouts[i].getDate().toString("dd-MMM-yyyy")));
        workoutsTable->setItem(row, 3, new QTableWidgetItem(workouts[i].getExerciseName()));
        workoutsTable->setItem(row, 4, new QTableWidgetItem(QString::number(workouts[i].getSets())));
        workoutsTable->setItem(row, 5, new QTableWidgetItem(QString::number(workouts[i].getReps())));
        workoutsTable->setItem(row, 6, new QTableWidgetItem(QString::number(workouts[i].getWeight(), 'f', 1)));
        workoutsTable->setItem(row, 7, new QTableWidgetItem(workouts[i].getCompleted() ? "✓ Completed" : "Pending"));
        if (workouts[i].getCompleted()) {
            for (int c = 0; c < 8; ++c) workoutsTable->item(row, c)->setForeground(QColor("#06d6a0"));
        }
    }
    delete[] workouts;
}

void MainWindow::populateSessionsTable() {
    sessionsTable->setRowCount(0);
    int count = 0;
    Session* sessions = gymManager->getAllSessions(count);
    if (!sessions) return;
    for (int i = 0; i < count; ++i) {
        int row = sessionsTable->rowCount();
        sessionsTable->insertRow(row);
        sessionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(sessions[i].getSessionId())));
        sessionsTable->setItem(row, 1, new QTableWidgetItem(sessions[i].getMemberName()));
        sessionsTable->setItem(row, 2, new QTableWidgetItem(sessions[i].getTrainerName()));
        sessionsTable->setItem(row, 3, new QTableWidgetItem(sessions[i].getSessionDate().toString("dd-MMM-yyyy")));
        sessionsTable->setItem(row, 4, new QTableWidgetItem(sessions[i].getStartTime().toString("HH:mm") + " - " + sessions[i].getEndTime().toString("HH:mm")));
        sessionsTable->setItem(row, 5, new QTableWidgetItem(sessions[i].getSessionType()));
        sessionsTable->setItem(row, 6, new QTableWidgetItem(sessions[i].getStatus()));
        if (sessions[i].getStatus() == "Completed") {
            for (int c = 0; c < 7; ++c) sessionsTable->item(row, c)->setForeground(QColor("#06d6a0"));
        }
        else if (sessions[i].getStatus() == "Cancelled") {
            for (int c = 0; c < 7; ++c) sessionsTable->item(row, c)->setForeground(QColor("#f77f00"));
        }
    }
    delete[] sessions;
}

void MainWindow::onSearchMemberById() {
    QString searchText = memberSearchByIdBox->text().trimmed();

    if (searchText.isEmpty()) {
        populateMembersTable();
        return;
    }

    bool ok;
    int memberId = searchText.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid numeric Member ID.");
        return;
    }

    membersTable->setRowCount(0);

    Member member = gymManager->getMember(memberId);

    if (member.getMemberId() == 0) {
        QMessageBox::information(this, "Not Found", QString("No member found with ID: %1").arg(memberId));
        return;
    }

    int row = membersTable->rowCount();
    membersTable->insertRow(row);
    membersTable->setItem(row, 0, new QTableWidgetItem(QString::number(member.getMemberId())));
    membersTable->setItem(row, 1, new QTableWidgetItem(member.getName()));
    membersTable->setItem(row, 2, new QTableWidgetItem(member.getPhone()));
    membersTable->setItem(row, 3, new QTableWidgetItem(member.getEmail()));
    membersTable->setItem(row, 4, new QTableWidgetItem(member.getMembershipPlan()));
    membersTable->setItem(row, 5, new QTableWidgetItem(member.getWantsTrainer() ? "Yes" : "No"));

    QString dietText = member.getDietPlan();
    if (dietText.compare("None") == 0) dietText = "—";
    else if (dietText.compare("Bulk (Gain Muscle)") == 0) dietText = "Bulk";
    else if (dietText.compare("Cut (Lose Fat)") == 0) dietText = "Cut";
    membersTable->setItem(row, 6, new QTableWidgetItem(dietText));

    membersTable->setItem(row, 7, new QTableWidgetItem(member.getStatusString()));
    membersTable->setItem(row, 8, new QTableWidgetItem(QString::number(member.getTotalFeesPaid(), 'f', 0)));
    membersTable->setItem(row, 9, new QTableWidgetItem(QString::number(member.getPendingFees(), 'f', 0)));

    if (!member.getIsActive()) {
        for (int c = 0; c < 10; ++c) {
            membersTable->item(row, c)->setForeground(QColor("#666"));
        }
    }
}

void MainWindow::onSearchMemberByName() {
    QString searchText = memberSearchByNameBox->text().trimmed();

    if (searchText.isEmpty()) {
        populateMembersTable();
        return;
    }

    membersTable->setRowCount(0);

    int count = 0;
    Member* results = gymManager->searchMembersByName(searchText, count);

    if (!results || count == 0) {
        QMessageBox::information(this, "Not Found", QString("No members found with name containing: %1").arg(searchText));
        return;
    }

    for (int i = 0; i < count; ++i) {
        int row = membersTable->rowCount();
        membersTable->insertRow(row);
        membersTable->setItem(row, 0, new QTableWidgetItem(QString::number(results[i].getMemberId())));
        membersTable->setItem(row, 1, new QTableWidgetItem(results[i].getName()));
        membersTable->setItem(row, 2, new QTableWidgetItem(results[i].getPhone()));
        membersTable->setItem(row, 3, new QTableWidgetItem(results[i].getEmail()));
        membersTable->setItem(row, 4, new QTableWidgetItem(results[i].getMembershipPlan()));
        membersTable->setItem(row, 5, new QTableWidgetItem(results[i].getWantsTrainer() ? "Yes" : "No"));

        QString dietText = results[i].getDietPlan();
        if (dietText.compare("None") == 0) dietText = "—";
        else if (dietText.compare("Bulk (Gain Muscle)") == 0) dietText = "Bulk";
        else if (dietText.compare("Cut (Lose Fat)") == 0) dietText = "Cut";
        membersTable->setItem(row, 6, new QTableWidgetItem(dietText));

        membersTable->setItem(row, 7, new QTableWidgetItem(results[i].getStatusString()));
        membersTable->setItem(row, 8, new QTableWidgetItem(QString::number(results[i].getTotalFeesPaid(), 'f', 0)));
        membersTable->setItem(row, 9, new QTableWidgetItem(QString::number(results[i].getPendingFees(), 'f', 0)));

        if (!results[i].getIsActive()) {
            for (int c = 0; c < 10; ++c) {
                membersTable->item(row, c)->setForeground(QColor("#666"));
            }
        }
    }
    delete[] results;
}

void MainWindow::onDeleteMember() {
    int row = membersTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "", "Please select a member.");
        return;
    }

    int id = membersTable->item(row, 0)->text().toInt();
    QString name = membersTable->item(row, 1)->text();

    int confirm = QMessageBox::question(this, "Confirm Delete",
        QString("Delete member '%1' (ID: %2)?\n\nThis will also delete all:\n"
            "• Payment records\n"
            "• Attendance records\n"
            "• Workout logs\n"
            "• Session bookings\n\n"
            "This cannot be undone.")
        .arg(name).arg(id),
        QMessageBox::Yes | QMessageBox::No);

    if (confirm == QMessageBox::Yes) {
        if (gymManager->deleteMember(id)) {
            QMessageBox::information(this, "Success", "Member deleted successfully!");
            populateMembersTable();
            populatePaymentsTable();
            populateAttendanceTable();
            populateWorkoutsTable();
            populateSessionsTable();
            updateDashboard();
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to delete member.");
        }
    }
}

void MainWindow::onDeleteAllMembers() {
    int reply = QMessageBox::question(this, "Delete All Members",
        "⚠️ WARNING: This will delete ALL members from the system!\n\n"
        "This action cannot be undone.\n\n"
        "Are you ABSOLUTELY sure you want to continue?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    int secondReply = QMessageBox::question(this, "Final Confirmation",
        "Click YES to permanently delete ALL members:",
        QMessageBox::Yes | QMessageBox::No);

    if (secondReply != QMessageBox::Yes) {
        return;
    }

    // Get all members
    int count = 0;
    Member* members = gymManager->getAllMembers(count);

    if (!members || count == 0) {
        QMessageBox::information(this, "No Members", "There are no members to delete.");
        return;
    }

    int deletedCount = 0;
    for (int i = 0; i < count; ++i) {
        if (members[i].getMemberId() > 0) {
            if (gymManager->deleteMember(members[i].getMemberId())) {
                deletedCount++;
            }
        }
    }
    delete[] members;

    // Force clear the table
    membersTable->setRowCount(0);

    // Refresh everything
    populateMembersTable();
    populatePaymentsTable();
    populateAttendanceTable();
    populateWorkoutsTable();
    populateSessionsTable();
    updateDashboard();

    QMessageBox::information(this, "Delete Complete",
        QString("Successfully deleted %1 members.\n\nTotal members now: 0")
        .arg(deletedCount));
}

void MainWindow::showMemberDialog(int memberId) {
    bool isEdit = (memberId != -1);
    Member existing;
    if (isEdit) existing = gymManager->getMember(memberId);
    QDialog dlg(this);
    dlg.setWindowTitle(isEdit ? "Edit Member" : "Add New Member");
    dlg.setMinimumWidth(500);
    dlg.setStyleSheet(styleSheet());
    QFormLayout* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 16, 20, 16);
    form->setSpacing(10);
    QLineEdit* leName = new QLineEdit(isEdit ? existing.getName() : "");
    QLineEdit* leCnic = new QLineEdit(isEdit ? existing.getCnic() : "");
    QLineEdit* lePhone = new QLineEdit(isEdit ? existing.getPhone() : "");
    QLineEdit* leEmail = new QLineEdit(isEdit ? existing.getEmail() : "");
    QLineEdit* leAddr = new QLineEdit(isEdit ? existing.getAddress() : "");
    QComboBox* cbGender = new QComboBox();
    cbGender->addItems({ "Male", "Female", "Other" });
    QDateEdit* deDob = new QDateEdit(isEdit ? existing.getDateOfBirth() : QDate(2000, 1, 1));
    deDob->setCalendarPopup(true);
    QComboBox* cbPlan = new QComboBox();
    cbPlan->addItems({ "Basic", "Standard", "Premium" });
    if (isEdit) cbPlan->setCurrentText(existing.getMembershipPlan());
    QCheckBox* chkWantsTrainer = new QCheckBox("I want a personal trainer (+Rs. 1000/month)");
    chkWantsTrainer->setChecked(isEdit ? existing.getWantsTrainer() : false);
    chkWantsTrainer->setEnabled(false);
    QLabel* lblDietPlan = new QLabel("Diet Plan (Premium only):");
    QComboBox* cbDietPlan = new QComboBox();
    cbDietPlan->addItems({ "None", "Bulk (Gain Muscle)", "Cut (Lose Fat)" });
    cbDietPlan->setEnabled(false);
    if (isEdit && existing.getMembershipPlan().compare("Premium") == 0) {
        cbDietPlan->setCurrentText(existing.getDietPlan());
        cbDietPlan->setEnabled(true);
        chkWantsTrainer->setEnabled(true);
    }
    QLabel* lblTotalDue = new QLabel();
    lblTotalDue->setStyleSheet("color: #e94560; font-weight: bold;");

    QObject::connect(cbPlan, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int) {
        QString plan = cbPlan->currentText();
        bool isStandardOrPremium = (plan.compare("Standard") == 0 || plan.compare("Premium") == 0);
        chkWantsTrainer->setEnabled(isStandardOrPremium);
        bool isPremium = (plan.compare("Premium") == 0);
        cbDietPlan->setEnabled(isPremium);
        if (!isPremium) cbDietPlan->setCurrentIndex(0);
        double totalDue = Member::getPlanFee(plan);
        if (chkWantsTrainer->isChecked() && isStandardOrPremium) totalDue += Member::getTrainerExtraFee();
        lblTotalDue->setText(QString("Total Due: Rs. %1").arg(totalDue, 0, 'f', 0));
        });

    QObject::connect(chkWantsTrainer, &QCheckBox::toggled, [=](bool checked) {
        QString plan = cbPlan->currentText();
        double totalDue = Member::getPlanFee(plan);
        if (checked && (plan.compare("Standard") == 0 || plan.compare("Premium") == 0)) {
            totalDue += Member::getTrainerExtraFee();
        }
        lblTotalDue->setText(QString("Total Due: Rs. %1").arg(totalDue, 0, 'f', 0));
        });

    leCnic->setPlaceholderText("13 digits (e.g., 1234567890123)");
    lePhone->setPlaceholderText("11 digits (e.g., 03123456789)");
    QLabel* lblCnicStatus = new QLabel();
    QLabel* lblPhoneStatus = new QLabel();
    lblCnicStatus->setStyleSheet("color: #f77f00; font-size: 10px;");
    lblPhoneStatus->setStyleSheet("color: #f77f00; font-size: 10px;");
    lblCnicStatus->setVisible(false);
    lblPhoneStatus->setVisible(false);

    QObject::connect(leCnic, &QLineEdit::textChanged, [lblCnicStatus](const QString& text) {
        if (!text.isEmpty()) {
            if (!Member::isValidCnic(text)) {
                lblCnicStatus->setText("❌ " + Member::getCnicValidationMessage());
                lblCnicStatus->setVisible(true);
                lblCnicStatus->setStyleSheet("color: #f77f00; font-size: 10px;");
            }
            else {
                lblCnicStatus->setText("✅ Valid CNIC");
                lblCnicStatus->setVisible(true);
                lblCnicStatus->setStyleSheet("color: #06d6a0; font-size: 10px;");
            }
        }
        else { lblCnicStatus->setVisible(false); }
        });

    QObject::connect(lePhone, &QLineEdit::textChanged, [lblPhoneStatus](const QString& text) {
        if (!text.isEmpty()) {
            if (!Member::isValidPhone(text)) {
                lblPhoneStatus->setText("❌ " + Member::getPhoneValidationMessage());
                lblPhoneStatus->setVisible(true);
                lblPhoneStatus->setStyleSheet("color: #f77f00; font-size: 10px;");
            }
            else {
                lblPhoneStatus->setText("✅ Valid Phone Number");
                lblPhoneStatus->setVisible(true);
                lblPhoneStatus->setStyleSheet("color: #06d6a0; font-size: 10px;");
            }
        }
        else { lblPhoneStatus->setVisible(false); }
        });

    QObject::connect(leCnic, &QLineEdit::textChanged, [leCnic](const QString& text) {
        QString filtered;
        for (QChar ch : text) if (ch.isDigit()) filtered.append(ch);
        if (filtered != text) {
            leCnic->blockSignals(true);
            leCnic->setText(filtered);
            leCnic->blockSignals(false);
        }
        if (filtered.length() > 13) {
            leCnic->blockSignals(true);
            leCnic->setText(filtered.left(13));
            leCnic->blockSignals(false);
        }
        });

    QObject::connect(lePhone, &QLineEdit::textChanged, [lePhone](const QString& text) {
        QString filtered;
        for (QChar ch : text) if (ch.isDigit()) filtered.append(ch);
        if (filtered != text) {
            lePhone->blockSignals(true);
            lePhone->setText(filtered);
            lePhone->blockSignals(false);
        }
        if (filtered.length() > 11) {
            lePhone->blockSignals(true);
            lePhone->setText(filtered.left(11));
            lePhone->blockSignals(false);
        }
        });

    form->addRow("Full Name *", leName);
    form->addRow("CNIC (13 digits)", leCnic);
    form->addRow("", lblCnicStatus);
    form->addRow("Phone * (11 digits)", lePhone);
    form->addRow("", lblPhoneStatus);
    form->addRow("Email", leEmail);
    form->addRow("Address", leAddr);
    form->addRow("Gender", cbGender);
    form->addRow("Date of Birth", deDob);
    form->addRow("Plan *", cbPlan);
    form->addRow("", chkWantsTrainer);
    form->addRow(lblDietPlan, cbDietPlan);
    form->addRow("", lblTotalDue);

    QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btns->button(QDialogButtonBox::Ok)->setText(isEdit ? "Update" : "Register");
    btns->button(QDialogButtonBox::Ok)->setObjectName("actionBtn");
    form->addRow(btns);

    QObject::connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    double initialDue = Member::getPlanFee(cbPlan->currentText());
    lblTotalDue->setText(QString("Total Due: Rs. %1").arg(initialDue, 0, 'f', 0));

    if (dlg.exec() != QDialog::Accepted) return;

    QString cnic = leCnic->text().trimmed();
    QString phone = lePhone->text().trimmed();
    QString name = leName->text().trimmed();
    QString email = leEmail->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Name is required.");
        return;
    }

    if (!cnic.isEmpty() && !Member::isValidCnic(cnic)) {
        QMessageBox::warning(this, "Validation", "Invalid CNIC!\n" + Member::getCnicValidationMessage());
        return;
    }

    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Phone number is required.");
        return;
    }

    if (!Member::isValidPhone(phone)) {
        QMessageBox::warning(this, "Validation", "Invalid Phone Number!\n" + Member::getPhoneValidationMessage());
        return;
    }

    if (!email.isEmpty() && !Member::isValidEmail(email)) {
        QMessageBox::warning(this, "Validation", "Invalid Email format!\nEmail should be like: name@domain.com");
        return;
    }

    QString selectedPlan = cbPlan->currentText();
    bool wantsTrainer = chkWantsTrainer->isChecked();
    QString dietPlan = cbDietPlan->currentText();

    if (selectedPlan.compare("Premium") == 0 && dietPlan.compare("None") == 0) {
        int reply = QMessageBox::question(this, "Diet Plan",
            "Premium members can choose a diet plan.\n• Bulk - For muscle gain\n• Cut - For fat loss\n\nDo you want to select a diet plan?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) return;
    }

    if (isEdit) {
        existing.setName(name);
        existing.setCnic(cnic);
        existing.setPhone(phone);
        existing.setEmail(email);
        existing.setAddress(leAddr->text().trimmed());
        existing.setMembershipPlan(selectedPlan);
        existing.setWantsTrainer(wantsTrainer);
        existing.setDietPlan(dietPlan);

        double newTotalDue = Member::getPlanFee(selectedPlan);
        if (wantsTrainer && (selectedPlan.compare("Standard") == 0 || selectedPlan.compare("Premium") == 0)) {
            newTotalDue += Member::getTrainerExtraFee();
        }
        existing.setPendingFees(newTotalDue);
        gymManager->updateMember(existing);
        QMessageBox::information(this, "Success", "Member updated.");
    }
    else {
        Member m(0, name, cnic, phone, email, leAddr->text().trimmed(),
            cbGender->currentText(), deDob->date(), selectedPlan, wantsTrainer, dietPlan);
        if (!gymManager->registerMember(m)) {
            QMessageBox::warning(this, "Validation", "Invalid email format.");
            return;
        }
        double totalDue = m.getTotalDue();
        QMessageBox::information(this, "Success",
            QString("Member registered! ID: %1\n\nTotal Due: Rs. %2\nPlease complete the payment to activate membership.")
            .arg(m.getMemberId()).arg(totalDue, 0, 'f', 0));
    }
    populateMembersTable();
}
