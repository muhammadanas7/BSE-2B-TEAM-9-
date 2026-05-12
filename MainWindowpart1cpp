#include "MainWindow.h"
#include "LoginWindow.h"
#include <QFile>
#include <QDir>
#include <QLocale>
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

MainWindowPart1::MainWindowPart1(QWidget* parent) : QMainWindow(parent) {
    gymManager = new GymManager("data");
    setWindowTitle("Gym Management System – Team #9 | BSE-2B");
    setMinimumSize(1100, 700);
    resize(1200, 750);
    setStyleSheet(styleSheet());
    buildLayout();
    showDashboard();
}

MainWindowPart1::~MainWindowPart1() { delete gymManager; }

void MainWindowPart1::setCurrentUser(const QString& username, const QString& role) {
    currentUsername = username;
    currentRole = role;
    if (lblUserInfo) lblUserInfo->setText(QString("👤 %1 (%2)").arg(username).arg(role));
    updatePermissions();
}

void MainWindowPart1::updatePermissions() { }

void MainWindowPart1::onLogout() {
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

QString MainWindowPart1::styleSheet() {
    return R"(
        QMainWindow, QDialog { background: #1a1a2e; }
        QWidget#sidebar { background: #16213e; border-right: 2px solid #0f3460; min-width: 200px; max-width: 200px; }
        QLabel#sidebarTitle { color: #e94560; font-size: 15px; font-weight: bold; padding: 18px 12px 6px 12px; }
        QLabel#sidebarSub { color: #888; font-size: 10px; padding: 0 12px 18px 12px; }
        QPushButton#navBtn { background: transparent; color: #a8b2d8; text-align: left; padding: 12px 20px; border: none; border-left: 3px solid transparent; }
        QPushButton#navBtn:hover { background: #0f3460; color: #e94560; border-left: 3px solid #e94560; }
        QPushButton#navBtn:checked { background: #0f3460; color: #e94560; border-left: 3px solid #e94560; font-weight: bold; }
        QWidget#page { background: #1a1a2e; }
        QLabel#pageTitle { color: #e94560; font-size: 20px; font-weight: bold; padding: 10px 0; }
        QFrame#statCard { background: #16213e; border: 1px solid #0f3460; border-radius: 10px; padding: 10px; }
        QLabel#statValue { color: #e94560; font-size: 28px; font-weight: bold; }
        QLabel#statLabel { color: #a8b2d8; font-size: 11px; }
        QTableWidget { background: #16213e; color: #ccd6f6; gridline-color: #0f3460; border: 1px solid #0f3460; border-radius: 6px; font-size: 12px; }
        QTableWidget::item:selected { background: #0f3460; color: #e94560; }
        QHeaderView::section { background: #0f3460; color: #e94560; font-weight: bold; padding: 6px; border: none; }
        QPushButton#actionBtn { background: #e94560; color: white; border: none; border-radius: 6px; padding: 8px 18px; font-size: 12px; font-weight: bold; }
        QPushButton#actionBtn:hover { background: #c73652; }
        QPushButton#secondaryBtn { background: #0f3460; color: #e94560; border: 1px solid #e94560; border-radius: 6px; padding: 8px 18px; font-size: 12px; }
        QPushButton#secondaryBtn:hover { background: #1a4a80; }
        QPushButton#dangerBtn { background: #8b0000; color: white; border: none; border-radius: 6px; padding: 8px 18px; font-size: 12px; }
        QPushButton#dangerBtn:hover { background: #a00000; }
        QLineEdit, QDateEdit, QTimeEdit, QComboBox { background: #16213e; color: #ccd6f6; border: 1px solid #0f3460; border-radius: 5px; padding: 6px 10px; font-size: 12px; }
        QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus, QComboBox:focus { border: 1px solid #e94560; }
        QTextEdit { background: #16213e; color: #ccd6f6; border: 1px solid #0f3460; border-radius: 6px; font-family: "Courier New"; font-size: 12px; }
        QGroupBox { color: #e94560; border: 1px solid #0f3460; border-radius: 8px; margin-top: 10px; font-size: 12px; font-weight: bold; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 6px; }
        QLabel { color: #ccd6f6; }
        QCheckBox { color: #ccd6f6; }
    )";
}

void MainWindowPart1::buildLayout() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    buildSidebar();
    mainLayout->addWidget(sidebar);
    stackedWidget = new QStackedWidget();
    
    // Build pages (to be overridden by child classes)
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

void MainWindowPart1::buildSidebar() {
    sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    QVBoxLayout* lay = new QVBoxLayout(sidebar);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    
    QLabel* title = new QLabel("💪 GYM MS");
    title->setObjectName("sidebarTitle");
    QLabel* sub = new QLabel("Team #9 | BSE-2B");
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
    };
    
    addNav("🏠", "Dashboard", &MainWindowPart1::showDashboard);
    addNav("👥", "Members", &MainWindowPart1::showMembers);
    addNav("🏋️", "Trainers", &MainWindowPart1::showTrainers);
    addNav("📋", "Plans", &MainWindowPart1::showPlans);
    addNav("✅", "Attendance", &MainWindowPart1::showAttendance);
    addNav("🔧", "Equipment", &MainWindowPart1::showEquipment);
    addNav("💳", "Payments", &MainWindowPart1::showPayments);
    addNav("📊", "Reports", &MainWindowPart1::showReports);
    addNav("🏋️", "Workouts
