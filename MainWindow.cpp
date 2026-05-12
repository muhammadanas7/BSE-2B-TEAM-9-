#include "MainWindow.h"
#include "LoginWindow.h"
#include <QFile>
#include <QDir>
#include <QLocale>
#include<Qstring>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>
#include <QGroupBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFrame>
#include <QCheckBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    gymManager = new GymManager("data");
    setWindowTitle("Gym Management System – Team #9 | BSE-2B");
    setMinimumSize(1100, 700);
    resize(1200, 750);
    setStyleSheet(styleSheet());
    buildLayout();
    showDashboard();
}

MainWindow::~MainWindow() { delete gymManager; }

void MainWindow::setCurrentUser(const QString& username, const QString& role) {
    currentUsername = username;
    currentRole = role;
    if (lblUserInfo) lblUserInfo->setText(QString("👤 %1 (%2)").arg(username).arg(role));
    updatePermissions();
}

void MainWindow::updatePermissions() {}

void MainWindow::onLogout() {
    this->hide();
    LoginWindow* loginWindow = new LoginWindow();
    connect(loginWindow, &LoginWindow::loginSuccessful, [this, loginWindow](const QString& username, const QString& role) {
        this->setCurrentUser(username, role);
        this->show();
        loginWindow->close();
        populateMembersTable();
        populateTrainersTable();
        populateAttendanceTable();
        populateEquipmentTable();
        populatePaymentsTable();
        populateWorkoutsTable();
        populateSessionsTable();
        updateDashboard();
        });
    loginWindow->show();
}

QString MainWindow::styleSheet() {
    return R"(
        QMainWindow { background: #1a1a2e; }

        QWidget#sidebar {
            background: #16213e;
            border-right: 2px solid #0f3460;
            min-width: 200px;
            max-width: 200px;
        }
        QLabel#sidebarTitle {
            color: #e94560;
            font-size: 15px;
            font-weight: bold;
            padding: 18px 12px 6px 12px;
        }
        QLabel#sidebarSub {
            color: #888;
            font-size: 10px;
            padding: 0 12px 18px 12px;
        }
        QPushButton#navBtn {
            background: transparent;
            color: #a8b2d8;
            text-align: left;
            padding: 12px 20px;
            border: none;
            font-size: 13px;
            border-left: 3px solid transparent;
        }
        QPushButton#navBtn:hover {
            background: #0f3460;
            color: #e94560;
            border-left: 3px solid #e94560;
        }
        QPushButton#navBtn:checked {
            background: #0f3460;
            color: #e94560;
            border-left: 3px solid #e94560;
            font-weight: bold;
        }

        QWidget#page {
            background: #1a1a2e;
        }
        QLabel#pageTitle {
            color: #e94560;
            font-size: 20px;
            font-weight: bold;
            padding: 10px 0;
        }

        QFrame#statCard {
            background: #16213e;
            border: 1px solid #0f3460;
            border-radius: 10px;
            padding: 10px;
        }
        QLabel#statValue {
            color: #e94560;
            font-size: 28px;
            font-weight: bold;
        }
        QLabel#statLabel {
            color: #a8b2d8;
            font-size: 11px;
        }

        QTableWidget {
            background: #16213e;
            color: #ccd6f6;
            gridline-color: #0f3460;
            border: 1px solid #0f3460;
            border-radius: 6px;
            font-size: 12px;
        }
        QTableWidget::item:selected {
            background: #0f3460;
            color: #e94560;
        }
        QHeaderView::section {
            background: #0f3460;
            color: #e94560;
            font-weight: bold;
            padding: 6px;
            border: none;
        }
        QTableWidget::item { padding: 4px 8px; }

        QPushButton#actionBtn {
            background: #e94560;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 18px;
            font-size: 12px;
            font-weight: bold;
        }
        QPushButton#actionBtn:hover { background: #c73652; }
        QPushButton#actionBtn:pressed { background: #a02a42; }

        QPushButton#secondaryBtn {
            background: #0f3460;
            color: #e94560;
            border: 1px solid #e94560;
            border-radius: 6px;
            padding: 8px 18px;
            font-size: 12px;
        }
        QPushButton#secondaryBtn:hover { background: #1a4a80; }

        QPushButton#dangerBtn {
            background: #8b0000;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 18px;
            font-size: 12px;
        }
        QPushButton#dangerBtn:hover { background: #a00000; }

        QLineEdit, QDateEdit, QComboBox {
            background: #16213e;
            color: #ccd6f6;
            border: 1px solid #0f3460;
            border-radius: 5px;
            padding: 6px 10px;
            font-size: 12px;
        }
        QLineEdit:focus, QDateEdit:focus, QComboBox:focus {
            border: 1px solid #e94560;
        }
        QComboBox QAbstractItemView {
            background: #16213e;
            color: #ccd6f6;
            selection-background-color: #0f3460;
        }

        QTextEdit {
            background: #16213e;
            color: #ccd6f6;
            border: 1px solid #0f3460;
            border-radius: 6px;
            font-family: "Courier New";
            font-size: 12px;
        }

        QGroupBox {
            color: #e94560;
            border: 1px solid #0f3460;
            border-radius: 8px;
            margin-top: 10px;
            font-size: 12px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 6px;
        }

        QScrollBar:vertical {
            background: #16213e;
            width: 8px;
        }
        QScrollBar::handle:vertical {
            background: #0f3460;
            border-radius: 4px;
        }

        QDialog {
            background: #1a1a2e;
            color: #ccd6f6;
        }
        QLabel { color: #ccd6f6; }
        QCheckBox { color: #ccd6f6; }
    )";
}

void MainWindow::buildLayout() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    buildSidebar();
    mainLayout->addWidget(sidebar);
    stackedWidget = new QStackedWidget();
    dashboardPage = buildDashboardPage();
    membersPage = buildMembersPage();
    trainersPage = buildTrainersPage();
    plansPage = buildPlansPage();
    attendancePage = buildAttendancePage();
    equipmentPage = buildEquipmentPage();
    paymentsPage = buildPaymentsPage();
    reportsPage = buildReportsPage();
    workoutsPage = buildWorkoutsPage();
    sessionsPage = buildSessionsPage();
    dietPlansPage = buildDietPlansPage();
    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(membersPage);
    stackedWidget->addWidget(trainersPage);
    stackedWidget->addWidget(plansPage);
    stackedWidget->addWidget(attendancePage);
    stackedWidget->addWidget(equipmentPage);
    stackedWidget->addWidget(paymentsPage);
    stackedWidget->addWidget(reportsPage);
    stackedWidget->addWidget(workoutsPage);
    stackedWidget->addWidget(sessionsPage);
    stackedWidget->addWidget(dietPlansPage);
    mainLayout->addWidget(stackedWidget);
}

void MainWindow::buildSidebar() {
    sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    QVBoxLayout* lay = new QVBoxLayout(sidebar);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    QLabel* title = new QLabel("💪 GYM MS");
    title->setObjectName("sidebarTitle");
    QLabel* sub = new QLabel("Team #9  |  BSE-2B");
    sub->setObjectName("sidebarSub");
    lay->addWidget(title);
    lay->addWidget(sub);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #0f3460;");
    lay->addWidget(line);

    auto addNav = [&](const QString& icon, const QString& text, auto slot) {
        QPushButton* btn = new QPushButton(icon + "  " + text);
        btn->setObjectName("navBtn");
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        connect(btn, &QPushButton::clicked, this, slot);
        lay->addWidget(btn);
        return btn;
        };

    addNav("🏠", "Dashboard", &MainWindow::showDashboard);
    addNav("👥", "Members", &MainWindow::showMembers);
    addNav("🏋️", "Trainers", &MainWindow::showTrainers);
    addNav("📋", "Plans", &MainWindow::showPlans);
    addNav("✅", "Attendance", &MainWindow::showAttendance);
    addNav("🔧", "Equipment", &MainWindow::showEquipment);
    addNav("💳", "Payments", &MainWindow::showPayments);
    addNav("📊", "Reports", &MainWindow::showReports);
    addNav("🏋️", "Workouts", &MainWindow::showWorkouts);
    addNav("📅", "Sessions", &MainWindow::showSessions);
    addNav("🥗", "Diet Plans", &MainWindow::showDietPlans);

    lay->addStretch();

    QFrame* userFrame = new QFrame();
    userFrame->setStyleSheet("background: #0f3460; margin: 10px; border-radius: 8px;");
    QVBoxLayout* userLayout = new QVBoxLayout(userFrame);

    lblUserInfo = new QLabel("👤 User");
    lblUserInfo->setStyleSheet("color: #e94560; font-size: 11px; padding: 5px;");
    lblUserInfo->setAlignment(Qt::AlignCenter);

    btnLogout = new QPushButton("🚪 Logout");
    btnLogout->setObjectName("secondaryBtn");
    btnLogout->setStyleSheet("QPushButton { padding: 6px; font-size: 11px; }");
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);

    userLayout->addWidget(lblUserInfo);
    userLayout->addWidget(btnLogout);
    lay->addWidget(userFrame);
    btnLogout = new QPushButton("🚪 Logout");
    btnLogout->setObjectName("secondaryBtn");
    btnLogout->setCursor(Qt::PointingHandCursor);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    lay->addWidget(btnLogout);
    QLabel* footer = new QLabel("FAST-NUCES\nCS1004 OOP Project");
    footer->setStyleSheet("color:#555; font-size:10px; padding:12px;");
    footer->setAlignment(Qt::AlignCenter);
    lay->addWidget(footer);
}
QWidget* MainWindow::buildDashboardPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(30, 20, 30, 20);
    QLabel* title = new QLabel("🏠  Dashboard");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QHBoxLayout* cardsRow = new QHBoxLayout();
    auto makeCard = [&](const QString& label, QLabel*& valLabel, const QString& color) {
        QFrame* card = new QFrame();
        card->setObjectName("statCard");
        card->setMinimumHeight(110);
        QVBoxLayout* cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 12, 16, 12);
        valLabel = new QLabel("0");
        valLabel->setObjectName("statValue");
        valLabel->setStyleSheet(QString("color: %1; font-size: 30px; font-weight: bold;").arg(color));
        QLabel* lbl = new QLabel(label);
        lbl->setObjectName("statLabel");
        cl->addWidget(valLabel);
        cl->addWidget(lbl);
        cardsRow->addWidget(card);
        };
    makeCard("Total Members", lblTotalMembers, "#e94560");
    makeCard("Active Members", lblActiveMembers, "#00b4d8");
    makeCard("Trainers", lblTotalTrainers, "#06d6a0");
    makeCard("Today Check-ins", lblTodayCheckIns, "#ffd166");
    makeCard("Monthly Revenue (Rs.)", lblMonthlyRevenue, "#f77f00");
    lay->addLayout(cardsRow);
    QGroupBox* actBox = new QGroupBox("Quick Actions");
    QHBoxLayout* actRow = new QHBoxLayout(actBox);
    auto addAction = [&](const QString& text, auto slot) {
        QPushButton* btn = new QPushButton(text);
        btn->setObjectName("actionBtn");
        connect(btn, &QPushButton::clicked, this, slot);
        actRow->addWidget(btn);
        };
    addAction("➕ Add Member", &MainWindow::showMembers);
    addAction("➕ Add Trainer", &MainWindow::showTrainers);
    addAction("✅ Check In", &MainWindow::showAttendance);
    addAction("💳 Record Payment", &MainWindow::showPayments);
    addAction("📊 Reports", &MainWindow::showReports);
    lay->addWidget(actBox);
    lay->addStretch();
    return page;
}

QWidget* MainWindow::buildMembersPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);

    QLabel* title = new QLabel("👥  Members");
    title->setObjectName("pageTitle");
    lay->addWidget(title);

    // Search by ID section
    QGroupBox* idSearchBox = new QGroupBox("Search by Member ID");
    QHBoxLayout* idSearchLayout = new QHBoxLayout(idSearchBox);

    memberSearchByIdBox = new QLineEdit();
    memberSearchByIdBox->setPlaceholderText("Enter Member ID");
    memberSearchByIdBox->setFixedWidth(150);

    btnSearchById = new QPushButton("🔍 Search by ID");
    btnSearchById->setObjectName("secondaryBtn");
    connect(btnSearchById, &QPushButton::clicked, this, &MainWindow::onSearchMemberById);

    idSearchLayout->addWidget(memberSearchByIdBox);
    idSearchLayout->addWidget(btnSearchById);
    idSearchLayout->addStretch();
    lay->addWidget(idSearchBox);

    // Search by Name section
    QGroupBox* nameSearchBox = new QGroupBox("Search by Member Name");
    QHBoxLayout* nameSearchLayout = new QHBoxLayout(nameSearchBox);

    memberSearchByNameBox = new QLineEdit();
    memberSearchByNameBox->setPlaceholderText("Enter Member Name (partial or full)");
    memberSearchByNameBox->setFixedWidth(250);

    btnSearchByName = new QPushButton("🔍 Search by Name");
    btnSearchByName->setObjectName("secondaryBtn");
    connect(btnSearchByName, &QPushButton::clicked, this, &MainWindow::onSearchMemberByName);

    QPushButton* btnResetSearch = new QPushButton("🔄 Show All Members");
    btnResetSearch->setObjectName("secondaryBtn");
    connect(btnResetSearch, &QPushButton::clicked, this, &MainWindow::populateMembersTable);

    nameSearchLayout->addWidget(memberSearchByNameBox);
    nameSearchLayout->addWidget(btnSearchByName);
    nameSearchLayout->addWidget(btnResetSearch);
    nameSearchLayout->addStretch();
    lay->addWidget(nameSearchBox);

    // Action buttons
    QHBoxLayout* toolbar = new QHBoxLayout();

    QPushButton* btnAdd = new QPushButton("➕ Add Member");
    QPushButton* btnEdit = new QPushButton("✏️ Edit");
    QPushButton* btnDel = new QPushButton("🗑 Delete");
    QPushButton* btnDeleteAllMembers = new QPushButton("⚠️ Delete All Members");
    QPushButton* btnRefresh = new QPushButton("🔄 Refresh");

    btnAdd->setObjectName("actionBtn");
    btnEdit->setObjectName("secondaryBtn");
    btnDel->setObjectName("dangerBtn");
    btnDeleteAllMembers->setObjectName("dangerBtn");
    btnRefresh->setObjectName("secondaryBtn");

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddMember);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditMember);
    connect(btnDel, &QPushButton::clicked, this, &MainWindow::onDeleteMember);
    connect(btnDeleteAllMembers, &QPushButton::clicked, this, &MainWindow::onDeleteAllMembers);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::populateMembersTable);

    toolbar->addStretch();
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDel);
    toolbar->addWidget(btnRefresh);
    toolbar->addWidget(btnDeleteAllMembers);
    lay->addLayout(toolbar);

    membersTable = new QTableWidget();
    membersTable->setColumnCount(10);
    membersTable->setHorizontalHeaderLabels({ "ID","Name","Phone","Email","Plan","Trainer","Diet","Status","Fees Paid","Pending" });
    membersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    membersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    membersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    membersTable->setAlternatingRowColors(true);
    membersTable->verticalHeader()->setVisible(false);
    connect(membersTable, &QTableWidget::cellClicked, this, &MainWindow::onMemberTableClicked);
    lay->addWidget(membersTable);

    return page;
}

QWidget* MainWindow::buildTrainersPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("🏋️  Trainers");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QHBoxLayout* toolbar = new QHBoxLayout();
    QPushButton* btnAdd = new QPushButton("➕ Add Trainer");
    QPushButton* btnEdit = new QPushButton("✏️ Edit");
    QPushButton* btnDel = new QPushButton("🗑 Remove");
    QPushButton* btnRefresh = new QPushButton("🔄 Refresh");
    btnAdd->setObjectName("actionBtn");
    btnEdit->setObjectName("secondaryBtn");
    btnDel->setObjectName("dangerBtn");
    btnRefresh->setObjectName("secondaryBtn");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddTrainer);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditTrainer);
    connect(btnDel, &QPushButton::clicked, this, &MainWindow::onDeleteTrainer);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::populateTrainersTable);
    toolbar->addStretch();
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDel);
    toolbar->addWidget(btnRefresh);
    lay->addLayout(toolbar);
    trainersTable = new QTableWidget();
    trainersTable->setColumnCount(7);
    trainersTable->setHorizontalHeaderLabels({ "ID","Name","Specialization","Shift","Phone","Salary","Status" });
    trainersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    trainersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    trainersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    trainersTable->setAlternatingRowColors(true);
    trainersTable->verticalHeader()->setVisible(false);
    lay->addWidget(trainersTable);
    return page;
}

QWidget* MainWindow::buildPlansPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("📋  Membership Plans");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    auto addPlanCard = [&](const QString& name, const QString& price, const QString& duration, const QString& features, const QString& color) {
        QFrame* card = new QFrame();
        card->setObjectName("statCard");
        card->setStyleSheet(QString("QFrame#statCard { border: 2px solid %1; border-radius:12px; background:#16213e; }").arg(color));
        QVBoxLayout* cl = new QVBoxLayout(card);
        cl->setContentsMargins(20, 16, 20, 16);
        QLabel* lName = new QLabel(name);
        lName->setStyleSheet(QString("color:%1; font-size:18px; font-weight:bold;").arg(color));
        QLabel* lPrice = new QLabel("Rs. " + price + " / month");
        lPrice->setStyleSheet("color:#ccd6f6; font-size:14px;");
        QLabel* lDur = new QLabel("Duration: " + duration);
        lDur->setStyleSheet("color:#888; font-size:11px;");
        QLabel* lFeat = new QLabel(features);
        lFeat->setStyleSheet("color:#a8b2d8; font-size:11px;");
        lFeat->setWordWrap(true);
        cl->addWidget(lName);
        cl->addWidget(lPrice);
        cl->addWidget(lDur);
        cl->addSpacing(8);
        cl->addWidget(lFeat);
        return card;
        };
    QHBoxLayout* row = new QHBoxLayout();
    row->addWidget(addPlanCard("Basic", "3,000", "1 Month", "✓ Gym Access\n✓ Locker\n✗ Trainer\n✗ Classes\n✗ Diet Plan", "#06d6a0"));
    row->addWidget(addPlanCard("Standard", "5,000", "3 Months", "✓ Gym Access\n✓ Locker\n✓ Optional Trainer (+Rs.1000)\n✓ Group Classes\n✗ Diet Plan", "#00b4d8"));
    row->addWidget(addPlanCard("Premium", "8,000", "6 Months", "✓ Gym Access\n✓ Locker\n✓ Optional Trainer (+Rs.1000)\n✓ All Classes\n✓ Diet Plan (Bulk/Cut)", "#e94560"));
    lay->addLayout(row);
    lay->addStretch();
    return page;
}

QWidget* MainWindow::buildAttendancePage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("✅  Attendance Tracking");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QGroupBox* ctrlBox = new QGroupBox("Record Attendance");
    QHBoxLayout* ctrlLay = new QHBoxLayout(ctrlBox);
    checkInIdBox = new QLineEdit();
    checkInIdBox->setPlaceholderText("Member ID");
    checkInIdBox->setFixedWidth(100);
    sessionCombo = new QComboBox();
    sessionCombo->addItems({ "Gym", "Yoga", "CrossFit", "Swimming", "Cardio" });
    QPushButton* btnIn = new QPushButton("✅ Check In");
    btnIn->setObjectName("actionBtn");
    connect(btnIn, &QPushButton::clicked, this, &MainWindow::onCheckIn);
    checkOutIdBox = new QLineEdit();
    checkOutIdBox->setPlaceholderText("Member ID");
    checkOutIdBox->setFixedWidth(100);
    QPushButton* btnOut = new QPushButton("🚪 Check Out");
    btnOut->setObjectName("secondaryBtn");
    connect(btnOut, &QPushButton::clicked, this, &MainWindow::onCheckOut);
    QPushButton* btnRef = new QPushButton("🔄 Refresh");
    btnRef->setObjectName("secondaryBtn");
    connect(btnRef, &QPushButton::clicked, this, &MainWindow::onRefreshAttendance);
    ctrlLay->addWidget(new QLabel("Member:"));
    ctrlLay->addWidget(checkInIdBox);
    ctrlLay->addWidget(new QLabel("Session:"));
    ctrlLay->addWidget(sessionCombo);
    ctrlLay->addWidget(btnIn);
    ctrlLay->addSpacing(20);
    ctrlLay->addWidget(new QLabel("Check Out ID:"));
    ctrlLay->addWidget(checkOutIdBox);
    ctrlLay->addWidget(btnOut);
    ctrlLay->addStretch();
    ctrlLay->addWidget(btnRef);
    lay->addWidget(ctrlBox);
    attendanceTable = new QTableWidget();
    attendanceTable->setColumnCount(7);
    attendanceTable->setHorizontalHeaderLabels({ "ID","Member","Date","Session","Check-In","Check-Out","Duration" });
    attendanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    attendanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    attendanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    attendanceTable->setAlternatingRowColors(true);
    attendanceTable->verticalHeader()->setVisible(false);
    lay->addWidget(attendanceTable);
    return page;
}

QWidget* MainWindow::buildEquipmentPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("🔧  Equipment Management");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QHBoxLayout* toolbar = new QHBoxLayout();
    QPushButton* btnAdd = new QPushButton("➕ Add Equipment");
    QPushButton* btnEdit = new QPushButton("✏️ Edit");
    QPushButton* btnDel = new QPushButton("🗑 Remove");
    QPushButton* btnRefresh = new QPushButton("🔄 Refresh");
    btnAdd->setObjectName("actionBtn");
    btnEdit->setObjectName("secondaryBtn");
    btnDel->setObjectName("dangerBtn");
    btnRefresh->setObjectName("secondaryBtn");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddEquipment);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditEquipment);
    connect(btnDel, &QPushButton::clicked, this, &MainWindow::onDeleteEquipment);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::populateEquipmentTable);
    toolbar->addStretch();
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDel);
    toolbar->addWidget(btnRefresh);
    lay->addLayout(toolbar);
    equipmentTable = new QTableWidget();
    equipmentTable->setColumnCount(8);
    equipmentTable->setHorizontalHeaderLabels({ "ID","Name","Category","Brand","Qty","Available","Condition","Status" });
    equipmentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    equipmentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    equipmentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    equipmentTable->setAlternatingRowColors(true);
    equipmentTable->verticalHeader()->setVisible(false);
    lay->addWidget(equipmentTable);
    return page;
}

QWidget* MainWindow::buildPaymentsPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("💳  Payments");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QHBoxLayout* toolbar = new QHBoxLayout();
    QPushButton* btnPay = new QPushButton("➕ Record Payment");
    QPushButton* btnReceipt = new QPushButton("🧾 View Receipt");
    QPushButton* btnRefresh = new QPushButton("🔄 Refresh");
    btnPay->setObjectName("actionBtn");
    btnReceipt->setObjectName("secondaryBtn");
    btnRefresh->setObjectName("secondaryBtn");
    connect(btnPay, &QPushButton::clicked, this, &MainWindow::onRecordPayment);
    connect(btnReceipt, &QPushButton::clicked, this, &MainWindow::onViewReceipt);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::populatePaymentsTable);
    toolbar->addStretch();
    toolbar->addWidget(btnPay);
    toolbar->addWidget(btnReceipt);
    toolbar->addWidget(btnRefresh);
    lay->addLayout(toolbar);
    paymentsTable = new QTableWidget();
    paymentsTable->setColumnCount(7);
    paymentsTable->setHorizontalHeaderLabels({ "ID","Member","Amount","Date","Method","Description","Status" });
    paymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    paymentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    paymentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    paymentsTable->setAlternatingRowColors(true);
    paymentsTable->verticalHeader()->setVisible(false);
    lay->addWidget(paymentsTable);
    return page;
}

QWidget* MainWindow::buildReportsPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("📊  Reports");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QGroupBox* ctrlBox = new QGroupBox("Generate Reports");
    QHBoxLayout* ctrlLay = new QHBoxLayout(ctrlBox);
    reportMemberIdBox = new QLineEdit();
    reportMemberIdBox->setPlaceholderText("Member ID");
    reportMemberIdBox->setFixedWidth(90);
    reportMonthCombo = new QComboBox();
    QLocale locale;
    for (int i = 1; i <= 12; ++i) reportMonthCombo->addItem(locale.monthName(i), i);
    reportMonthCombo->setCurrentIndex(QDate::currentDate().month() - 1);
    reportYearCombo = new QComboBox();
    int yr = QDate::currentDate().year();
    for (int y = yr - 2; y <= yr + 1; ++y) reportYearCombo->addItem(QString::number(y), y);
    reportYearCombo->setCurrentIndex(2);
    QPushButton* btnMember = new QPushButton("Member Report");
    QPushButton* btnRevenue = new QPushButton("Revenue Report");
    QPushButton* btnAttend = new QPushButton("Attendance Report");
    QPushButton* btnEquip = new QPushButton("Equipment Report");
    QPushButton* btnBackup = new QPushButton("💾 Backup Data");
    btnMember->setObjectName("actionBtn");
    btnRevenue->setObjectName("secondaryBtn");
    btnAttend->setObjectName("secondaryBtn");
    btnEquip->setObjectName("secondaryBtn");
    btnBackup->setObjectName("secondaryBtn");
    connect(btnMember, &QPushButton::clicked, this, &MainWindow::onGenerateMemberReport);
    connect(btnRevenue, &QPushButton::clicked, this, &MainWindow::onGenerateRevenueReport);
    connect(btnAttend, &QPushButton::clicked, this, [this]() {
        int m = reportMonthCombo->currentData().toInt();
        int y = reportYearCombo->currentData().toInt();
        reportOutput->setPlainText(gymManager->generateAttendanceReport(m, y));
        });
    connect(btnEquip, &QPushButton::clicked, this, [this]() {
        reportOutput->setPlainText(gymManager->generateEquipmentReport());
        });
    connect(btnBackup, &QPushButton::clicked, this, &MainWindow::onBackupData);
    ctrlLay->addWidget(new QLabel("Member ID:"));
    ctrlLay->addWidget(reportMemberIdBox);
    ctrlLay->addWidget(new QLabel("Month:"));
    ctrlLay->addWidget(reportMonthCombo);
    ctrlLay->addWidget(new QLabel("Year:"));
    ctrlLay->addWidget(reportYearCombo);
    ctrlLay->addWidget(btnMember);
    ctrlLay->addWidget(btnRevenue);
    ctrlLay->addWidget(btnAttend);
    ctrlLay->addWidget(btnEquip);
    ctrlLay->addWidget(btnBackup);
    lay->addWidget(ctrlBox);
    reportOutput = new QTextEdit();
    reportOutput->setReadOnly(true);
    reportOutput->setPlaceholderText("Report output will appear here...");
    lay->addWidget(reportOutput);
    return page;
}

QWidget* MainWindow::buildWorkoutsPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("🏋️  Workout Management");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QHBoxLayout* toolbar = new QHBoxLayout();
    QPushButton* btnAdd = new QPushButton("➕ Add Workout");
    QPushButton* btnEdit = new QPushButton("✏️ Edit");
    QPushButton* btnDel = new QPushButton("🗑 Delete");
    QPushButton* btnComplete = new QPushButton("✅ Mark Complete");
    QPushButton* btnRefresh = new QPushButton("🔄 Refresh");
    btnAdd->setObjectName("actionBtn");
    btnEdit->setObjectName("secondaryBtn");
    btnDel->setObjectName("dangerBtn");
    btnComplete->setObjectName("secondaryBtn");
    btnRefresh->setObjectName("secondaryBtn");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddWorkout);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditWorkout);
    connect(btnDel, &QPushButton::clicked, this, &MainWindow::onDeleteWorkout);
    connect(btnComplete, &QPushButton::clicked, this, &MainWindow::onCompleteWorkout);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::populateWorkoutsTable);
    toolbar->addStretch();
    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnComplete);
    toolbar->addWidget(btnDel);
    toolbar->addWidget(btnRefresh);
    lay->addLayout(toolbar);
    workoutsTable = new QTableWidget();
    workoutsTable->setColumnCount(8);
    workoutsTable->setHorizontalHeaderLabels({ "ID","Member","Date","Exercise","Sets","Reps","Weight","Status" });
    workoutsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    workoutsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    workoutsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    workoutsTable->setAlternatingRowColors(true);
    workoutsTable->verticalHeader()->setVisible(false);
    lay->addWidget(workoutsTable);
    return page;
}

QWidget* MainWindow::buildSessionsPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);
    QLabel* title = new QLabel("📅  Session Scheduling");
    title->setObjectName("pageTitle");
    lay->addWidget(title);
    QHBoxLayout* toolbar = new QHBoxLayout();
    QPushButton* btnSchedule = new QPushButton("📅 Schedule Session");
    QPushButton* btnComplete = new QPushButton("✅ Complete");
    QPushButton* btnCancel = new QPushButton("❌ Cancel");
    QPushButton* btnRefresh = new QPushButton("🔄 Refresh");
    btnSchedule->setObjectName("actionBtn");
    btnComplete->setObjectName("secondaryBtn");
    btnCancel->setObjectName("dangerBtn");
    btnRefresh->setObjectName("secondaryBtn");
    connect(btnSchedule, &QPushButton::clicked, this, &MainWindow::onScheduleSession);
    connect(btnComplete, &QPushButton::clicked, this, &MainWindow::onCompleteSession);
    connect(btnCancel, &QPushButton::clicked, this, &MainWindow::onCancelSession);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::populateSessionsTable);
    toolbar->addStretch();
    toolbar->addWidget(btnSchedule);
    toolbar->addWidget(btnComplete);
    toolbar->addWidget(btnCancel);
    toolbar->addWidget(btnRefresh);
    lay->addLayout(toolbar);
    sessionsTable = new QTableWidget();
    sessionsTable->setColumnCount(7);
    sessionsTable->setHorizontalHeaderLabels({ "ID","Member","Trainer","Date","Time","Type","Status" });
    sessionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sessionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    sessionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sessionsTable->setAlternatingRowColors(true);
    sessionsTable->verticalHeader()->setVisible(false);
    lay->addWidget(sessionsTable);
    return page;
}

QWidget* MainWindow::buildDietPlansPage() {
    QWidget* page = new QWidget();
    page->setObjectName("page");
    QVBoxLayout* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 15, 20, 15);

    QLabel* title = new QLabel("🥗  Diet Plans");
    title->setObjectName("pageTitle");
    lay->addWidget(title);

    QHBoxLayout* cardsRow = new QHBoxLayout();

    // Bulk Diet Plan Card
    QFrame* bulkCard = new QFrame();
    bulkCard->setObjectName("statCard");
    bulkCard->setStyleSheet("QFrame#statCard { border: 2px solid #06d6a0; border-radius:12px; background:#16213e; }");
    QVBoxLayout* bulkLayout = new QVBoxLayout(bulkCard);
    bulkLayout->setContentsMargins(20, 16, 20, 16);

    QLabel* bulkTitle = new QLabel("💪 BULK DIET PLAN");
    bulkTitle->setStyleSheet("color: #06d6a0; font-size: 18px; font-weight: bold;");
    bulkTitle->setAlignment(Qt::AlignCenter);

    QTextEdit* bulkDetails = new QTextEdit();
    bulkDetails->setReadOnly(true);
    bulkDetails->setMaximumHeight(400);
    bulkDetails->setPlainText(
        "════════════════════════════════════════\n"
        "         BULK DIET PLAN\n"
        "════════════════════════════════════════\n\n"
        "📌 DESCRIPTION:\n"
        "High protein, high carb diet for muscle gain\n\n"
        "📊 NUTRITION FACTS:\n"
        "  • Calories: 3000 kcal\n"
        "  • Protein: 180g\n"
        "  • Carbs: 350g\n"
        "  • Fats: 80g\n\n"
        "🍳 BREAKFAST:\n"
        "  6 eggs + 2 slices whole wheat bread + 1 banana\n\n"
        "🍗 LUNCH:\n"
        "  250g chicken breast + 1 cup rice + mixed vegetables\n\n"
        "🥩 DINNER:\n"
        "  250g beef + 2 boiled potatoes + vegetables\n\n"
        "🥜 SNACKS:\n"
        "  Protein shake + 1 apple + handful almonds\n\n"
        "════════════════════════════════════════"
    );
    bulkDetails->setStyleSheet("background: #0f3460; color: #ccd6f6; font-family: monospace; font-size: 11px;");

    bulkLayout->addWidget(bulkTitle);
    bulkLayout->addSpacing(10);
    bulkLayout->addWidget(bulkDetails);

    // Cut Diet Plan Card
    QFrame* cutCard = new QFrame();
    cutCard->setObjectName("statCard");
    cutCard->setStyleSheet("QFrame#statCard { border: 2px solid #e94560; border-radius:12px; background:#16213e; }");
    QVBoxLayout* cutLayout = new QVBoxLayout(cutCard);
    cutLayout->setContentsMargins(20, 16, 20, 16);

    QLabel* cutTitle = new QLabel("🔥 CUT DIET PLAN");
    cutTitle->setStyleSheet("color: #e94560; font-size: 18px; font-weight: bold;");
    cutTitle->setAlignment(Qt::AlignCenter);

    QTextEdit* cutDetails = new QTextEdit();
    cutDetails->setReadOnly(true);
    cutDetails->setMaximumHeight(400);
    cutDetails->setPlainText(
        "════════════════════════════════════════\n"
        "          CUT DIET PLAN\n"
        "════════════════════════════════════════\n\n"
        "📌 DESCRIPTION:\n"
        "High protein, low carb diet for fat loss\n\n"
        "📊 NUTRITION FACTS:\n"
        "  • Calories: 2000 kcal\n"
        "  • Protein: 160g\n"
        "  • Carbs: 120g\n"
        "  • Fats: 50g\n\n"
        "🍳 BREAKFAST:\n"
        "  4 eggs + oatmeal with berries\n\n"
        "🍗 LUNCH:\n"
        "  200g chicken breast + large salad + 1/2 cup rice\n\n"
        "🐟 DINNER:\n"
        "  200g fish + 2 cups vegetables\n\n"
        "🥜 SNACKS:\n"
        "  Protein shake + cucumber + yogurt\n\n"
        "════════════════════════════════════════"
    );
    cutDetails->setStyleSheet("background: #0f3460; color: #ccd6f6; font-family: monospace; font-size: 11px;");

    cutLayout->addWidget(cutTitle);
    cutLayout->addSpacing(10);
    cutLayout->addWidget(cutDetails);

    cardsRow->addWidget(bulkCard);
    cardsRow->addWidget(cutCard);
    lay->addLayout(cardsRow);

    QGroupBox* infoBox = new QGroupBox("⚠️ Important Note");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoBox);
    QLabel* infoText = new QLabel(
        "Diet plans are only available for PREMIUM members.\n"
        "Contact your trainer for personalized diet plan adjustments."
    );
    infoText->setStyleSheet("color: #f77f00; font-size: 12px;");
    infoText->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(infoText);
    lay->addWidget(infoBox);

    return page;
}

void MainWindow::showDashboard() { stackedWidget->setCurrentWidget(dashboardPage); updateDashboard(); }
void MainWindow::showMembers() { stackedWidget->setCurrentWidget(membersPage); populateMembersTable(); }
void MainWindow::showTrainers() { stackedWidget->setCurrentWidget(trainersPage); populateTrainersTable(); }
void MainWindow::showPlans() { stackedWidget->setCurrentWidget(plansPage); }
void MainWindow::showAttendance() { stackedWidget->setCurrentWidget(attendancePage); populateAttendanceTable(); }
void MainWindow::showEquipment() { stackedWidget->setCurrentWidget(equipmentPage); populateEquipmentTable(); }
void MainWindow::showPayments() { stackedWidget->setCurrentWidget(paymentsPage); populatePaymentsTable(); }
void MainWindow::showReports() { stackedWidget->setCurrentWidget(reportsPage); }
void MainWindow::showWorkouts() { stackedWidget->setCurrentWidget(workoutsPage); populateWorkoutsTable(); }
void MainWindow::showSessions() { stackedWidget->setCurrentWidget(sessionsPage); populateSessionsTable(); }
void MainWindow::showDietPlans() { stackedWidget->setCurrentWidget(dietPlansPage); }

void MainWindow::updateDashboard() {
    lblTotalMembers->setText(QString::number(gymManager->getTotalMembers()));
    lblActiveMembers->setText(QString::number(gymManager->getActiveMembers()));
    lblTotalTrainers->setText(QString::number(gymManager->getTotalTrainers()));
    lblTodayCheckIns->setText(QString::number(gymManager->getTodayCheckIns()));
    lblMonthlyRevenue->setText(QString::number(gymManager->getMonthlyRevenue(), 'f', 0));
}

void MainWindow::populateMembersTable() {
    membersTable->setRowCount(0);
    int count = 0;
    Member* members = gymManager->getAllMembers(count);

    // If no members, just clear table and return
    if (!members || count == 0) {
        membersTable->setRowCount(0);
        return;
    }

    // Only add rows for valid members (memberId > 0)
    for (int i = 0; i < count; ++i) {
        // Skip invalid members (ID = 0 or empty name)
        if (members[i].getMemberId() <= 0 || members[i].getName().isEmpty()) {
            continue;
        }

        int row = membersTable->rowCount();
        membersTable->insertRow(row);
        membersTable->setItem(row, 0, new QTableWidgetItem(QString::number(members[i].getMemberId())));
        membersTable->setItem(row, 1, new QTableWidgetItem(members[i].getName()));
        membersTable->setItem(row, 2, new QTableWidgetItem(members[i].getPhone()));
        membersTable->setItem(row, 3, new QTableWidgetItem(members[i].getEmail()));
        membersTable->setItem(row, 4, new QTableWidgetItem(members[i].getMembershipPlan()));
        membersTable->setItem(row, 5, new QTableWidgetItem(members[i].getWantsTrainer() ? "Yes" : "No"));

        QString dietText = members[i].getDietPlan();
        if (dietText == "None") dietText = "—";
        else if (dietText == "Bulk (Gain Muscle)") dietText = "Bulk";
        else if (dietText == "Cut (Lose Fat)") dietText = "Cut";
        membersTable->setItem(row, 6, new QTableWidgetItem(dietText));

        membersTable->setItem(row, 7, new QTableWidgetItem(members[i].getStatusString()));
        membersTable->setItem(row, 8, new QTableWidgetItem(QString::number(members[i].getTotalFeesPaid(), 'f', 0)));
        membersTable->setItem(row, 9, new QTableWidgetItem(QString::number(members[i].getPendingFees(), 'f', 0)));

        if (!members[i].getIsActive()) {
            for (int c = 0; c < 10; ++c) {
                membersTable->item(row, c)->setForeground(QColor("#666"));
            }
        }
    }
    delete[] members;
}

//

void MainWindow::populateTrainersTable() {
    trainersTable->setRowCount(0);
    int count = 0;
    Trainer* trainers = gymManager->getAllTrainers(count);
    if (!trainers) return;
    for (int i = 0; i < count; ++i) {
        int row = trainersTable->rowCount();
        trainersTable->insertRow(row);
        trainersTable->setItem(row, 0, new QTableWidgetItem(QString::number(trainers[i].getTrainerId())));
        trainersTable->setItem(row, 1, new QTableWidgetItem(trainers[i].getName()));
        trainersTable->setItem(row, 2, new QTableWidgetItem(trainers[i].getSpecialization()));
        trainersTable->setItem(row, 3, new QTableWidgetItem(trainers[i].getShift()));
        trainersTable->setItem(row, 4, new QTableWidgetItem(trainers[i].getPhone()));
        trainersTable->setItem(row, 5, new QTableWidgetItem(QString::number(trainers[i].getSalary(), 'f', 0)));
        trainersTable->setItem(row, 6, new QTableWidgetItem(trainers[i].getAvailabilityStatus()));
    }
    delete[] trainers;
}

void MainWindow::populateAttendanceTable() {
    attendanceTable->setRowCount(0);
    int count = 0;
    Attendance* att = gymManager->getTodayAttendance(count);
    if (!att) return;
    for (int i = 0; i < count; ++i) {
        int row = attendanceTable->rowCount();
        attendanceTable->insertRow(row);
        attendanceTable->setItem(row, 0, new QTableWidgetItem(QString::number(att[i].getRecordId())));
        attendanceTable->setItem(row, 1, new QTableWidgetItem(att[i].getMemberName()));
        attendanceTable->setItem(row, 2, new QTableWidgetItem(att[i].getDate().toString("dd-MMM-yyyy")));
        attendanceTable->setItem(row, 3, new QTableWidgetItem(att[i].getSessionType()));
        attendanceTable->setItem(row, 4, new QTableWidgetItem(att[i].getCheckIn().toString("HH:mm")));
        attendanceTable->setItem(row, 5, new QTableWidgetItem(att[i].isCheckOutRecorded() ? att[i].getCheckOut().toString("HH:mm") : "—"));
        attendanceTable->setItem(row, 6, new QTableWidgetItem(att[i].getDurationString()));
    }
    delete[] att;
}

void MainWindow::populateEquipmentTable() {
    equipmentTable->setRowCount(0);
    int count = 0;
    Equipment* equip = gymManager->getAllEquipment(count);
    if (!equip) return;
    for (int i = 0; i < count; ++i) {
        int row = equipmentTable->rowCount();
        equipmentTable->insertRow(row);
        equipmentTable->setItem(row, 0, new QTableWidgetItem(QString::number(equip[i].getEquipmentId())));
        equipmentTable->setItem(row, 1, new QTableWidgetItem(equip[i].getName()));
        equipmentTable->setItem(row, 2, new QTableWidgetItem(equip[i].getCategory()));
        equipmentTable->setItem(row, 3, new QTableWidgetItem(equip[i].getBrand()));
        equipmentTable->setItem(row, 4, new QTableWidgetItem(QString::number(equip[i].getQuantity())));
        equipmentTable->setItem(row, 5, new QTableWidgetItem(QString::number(equip[i].getAvailableQty())));
        equipmentTable->setItem(row, 6, new QTableWidgetItem(equip[i].getCondition()));
        equipmentTable->setItem(row, 7, new QTableWidgetItem(equip[i].getStatusString()));
        if (equip[i].isMaintenanceDue()) {
            for (int c = 0; c < 8; ++c) equipmentTable->item(row, c)->setForeground(QColor("#f77f00"));
        }
    }
    delete[] equip;
}


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
