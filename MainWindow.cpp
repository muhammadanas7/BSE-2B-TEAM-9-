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
