#include "LoginWindow.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QMessageBox>
#include <QDir>

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Gym Management System - Login");
    setFixedSize(600, 600);  // Changed from setMinimumSize and setFixedSize
    setModal(true);
    setStyleSheet(styleSheet());
    fileManager = new FileManager("data");
    stackedWidget = new QStackedWidget(this);
    setupLoginPage();
    setupSignupPage();
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
    stackedWidget->setCurrentWidget(loginPage);
}

LoginWindow::~LoginWindow() { delete fileManager; }

QString LoginWindow::styleSheet() {
    return R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1a1a2e, stop:1 #16213e);
        }
        QLabel#title {
            color: #e94560;
            font-size: 32px;
            font-weight: bold;
            font-family: 'Segoe UI';
            margin-top: 20px;
        }
        QLabel#subtitle {
            color: #a8b2d8;
            font-size: 14px;
            margin-bottom: 20px;
        }
        QLabel#errorLabel {
            color: #f77f00;
            font-size: 11px;
            margin-top: 5px;
        }
        QLineEdit, QComboBox {
            background: #0f3460;
            color: #ccd6f6;
            border: 1px solid #e94560;
            border-radius: 10px;
            padding: 12px 15px;
            font-size: 14px;
            min-width: 280px;
            margin: 5px 0;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 2px solid #e94560;
        }
        QPushButton#loginBtn {
            background: #e94560;
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px;
            font-size: 16px;
            font-weight: bold;
            min-width: 280px;
            margin-top: 15px;
        }
        QPushButton#loginBtn:hover {
            background: #c73652;
        }
        QPushButton#secondaryBtn {
            background: transparent;
            color: #e94560;
            border: 1px solid #e94560;
            border-radius: 10px;
            padding: 10px;
            font-size: 13px;
            min-width: 280px;
            margin-top: 10px;
        }
        QPushButton#secondaryBtn:hover {
            background: #e94560;
            color: white;
        }
        QFrame#card {
            background: #16213e;
            border-radius: 20px;
            padding: 40px;
            min-width: 400px;
        }
    )";
}

void LoginWindow::setupLoginPage() {
    loginPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);

    QFrame* card = new QFrame();
    card->setObjectName("card");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(15);

    QLabel* lblTitle = new QLabel("💪 GYM MANAGEMENT SYSTEM");
    lblTitle->setObjectName("title");
    lblTitle->setAlignment(Qt::AlignCenter);

    QLabel* lblSub = new QLabel("Login to your account");
    lblSub->setObjectName("subtitle");
    lblSub->setAlignment(Qt::AlignCenter);

    leLoginUsername = new QLineEdit();
    leLoginUsername->setPlaceholderText("Enter Username");
    leLoginUsername->setAlignment(Qt::AlignCenter);

    leLoginPassword = new QLineEdit();
    leLoginPassword->setPlaceholderText("Enter Password");
    leLoginPassword->setEchoMode(QLineEdit::Password);
    leLoginPassword->setAlignment(Qt::AlignCenter);

    btnLogin = new QPushButton("LOGIN");
    btnLogin->setObjectName("loginBtn");
    connect(btnLogin, &QPushButton::clicked, this, &LoginWindow::onLogin);

    lblLoginError = new QLabel();
    lblLoginError->setObjectName("errorLabel");
    lblLoginError->setAlignment(Qt::AlignCenter);
    lblLoginError->setVisible(false);

    btnGoToSignup = new QPushButton("Don't have an account? Sign Up");
    btnGoToSignup->setObjectName("secondaryBtn");
    connect(btnGoToSignup, &QPushButton::clicked, this, &LoginWindow::onShowSignup);

    cardLayout->addWidget(lblTitle);
    cardLayout->addWidget(lblSub);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(leLoginUsername);
    cardLayout->addWidget(leLoginPassword);
    cardLayout->addWidget(btnLogin);
    cardLayout->addWidget(lblLoginError);
    cardLayout->addWidget(btnGoToSignup);

    layout->addWidget(card);
    stackedWidget->addWidget(loginPage);
}

void LoginWindow::setupSignupPage() {
    signupPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(signupPage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);

    QFrame* card = new QFrame();
    card->setObjectName("card");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(12);

    QLabel* lblTitle = new QLabel("CREATE ACCOUNT");
    lblTitle->setObjectName("title");
    lblTitle->setAlignment(Qt::AlignCenter);

    leSignupUsername = new QLineEdit();
    leSignupUsername->setPlaceholderText("Choose Username *");
    leSignupUsername->setAlignment(Qt::AlignCenter);

    leSignupEmail = new QLineEdit();
    leSignupEmail->setPlaceholderText("Enter Email *");
    leSignupEmail->setAlignment(Qt::AlignCenter);

    leSignupPassword = new QLineEdit();
    leSignupPassword->setPlaceholderText("Create Password *");
    leSignupPassword->setEchoMode(QLineEdit::Password);
    leSignupPassword->setAlignment(Qt::AlignCenter);

    leSignupConfirmPassword = new QLineEdit();
    leSignupConfirmPassword->setPlaceholderText("Confirm Password *");
    leSignupConfirmPassword->setEchoMode(QLineEdit::Password);
    leSignupConfirmPassword->setAlignment(Qt::AlignCenter);

    cbRole = new QComboBox();
    cbRole->addItems({ "Admin", "Manager", "Staff" });

    btnSignup = new QPushButton("SIGN UP");
    btnSignup->setObjectName("loginBtn");
    connect(btnSignup, &QPushButton::clicked, this, &LoginWindow::onSignup);

    lblSignupError = new QLabel();
    lblSignupError->setObjectName("errorLabel");
    lblSignupError->setAlignment(Qt::AlignCenter);
    lblSignupError->setVisible(false);

    btnBackToLogin = new QPushButton("Already have an account? Login");
    btnBackToLogin->setObjectName("secondaryBtn");
    connect(btnBackToLogin, &QPushButton::clicked, this, &LoginWindow::onShowLogin);

    cardLayout->addWidget(lblTitle);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(leSignupUsername);
    cardLayout->addWidget(leSignupEmail);
    cardLayout->addWidget(leSignupPassword);
    cardLayout->addWidget(leSignupConfirmPassword);
    cardLayout->addWidget(cbRole);
    cardLayout->addWidget(btnSignup);
    cardLayout->addWidget(lblSignupError);
    cardLayout->addWidget(btnBackToLogin);

    layout->addWidget(card);
    stackedWidget->addWidget(signupPage);
}

bool LoginWindow::validateEmail(const QString& email) { return UserAuth::isValidEmail(email); }

bool LoginWindow::usernameExists(const QString& username) {
    int count = 0;
    UserAuth* users = fileManager->loadAllUsers(count);
    bool exists = false;
    for (int i = 0; i < count; ++i) {
        if (users[i].getUsername().toLower() == username.toLower()) { exists = true; break; }
    }
    delete[] users;
    return exists;
}

bool LoginWindow::emailExists(const QString& email) {
    int count = 0;
    UserAuth* users = fileManager->loadAllUsers(count);
    bool exists = false;
    for (int i = 0; i < count; ++i) {
        if (users[i].getEmail().toLower() == email.toLower()) { exists = true; break; }
    }
    delete[] users;
    return exists;
}

void LoginWindow::saveUser(const UserAuth& user) { fileManager->saveUser(user); }

bool LoginWindow::authenticateUser(const QString& username, const QString& password, UserAuth& outUser) {
    UserAuth user = fileManager->loadUser(username);
    if (user.getUserId() == 0) return false;
    if (!user.verifyPassword(password)) return false;
    if (!user.getIsActive()) return false;
    outUser = user;
    outUser.setLastLogin(QDate::currentDate());
    fileManager->updateUser(outUser);
    return true;
}

void LoginWindow::onLogin() {
    QString username = leLoginUsername->text().trimmed();
    QString password = leLoginPassword->text().trimmed();
    if (username.isEmpty() || password.isEmpty()) {
        lblLoginError->setText("Please enter username and password");
        lblLoginError->setVisible(true);
        return;
    }
    UserAuth user;
    if (authenticateUser(username, password, user)) {
        emit loginSuccessful(user.getUsername(), user.getRole());
        this->hide();
    }
    else {
        lblLoginError->setText("Invalid username or password");
        lblLoginError->setVisible(true);
    }
}

void LoginWindow::onSignup() {
    QString username = leSignupUsername->text().trimmed();
    QString email = leSignupEmail->text().trimmed();
    QString password = leSignupPassword->text().trimmed();
    QString confirm = leSignupConfirmPassword->text().trimmed();
    QString role = cbRole->currentText();
    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        lblSignupError->setText("All fields are required");
        lblSignupError->setVisible(true);
        return;
    }
    if (!validateEmail(email)) {
        lblSignupError->setText("Please enter a valid email address (e.g., name@domain.com)");
        lblSignupError->setVisible(true);
        return;
    }
    if (password.length() < 4) {
        lblSignupError->setText("Password must be at least 4 characters");
        lblSignupError->setVisible(true);
        return;
    }
    if (password != confirm) {
        lblSignupError->setText("Passwords do not match");
        lblSignupError->setVisible(true);
        return;
    }
    if (usernameExists(username)) {
        lblSignupError->setText("Username already exists");
        lblSignupError->setVisible(true);
        return;
    }
    if (emailExists(email)) {
        lblSignupError->setText("Email already registered");
        lblSignupError->setVisible(true);
        return;
    }
    int userId = fileManager->getNextUserId();
    UserAuth newUser(userId, username, password, email, role);
    saveUser(newUser);
    QMessageBox::information(this, "Success", "Account created successfully!\nYou can now login.");
    onShowLogin();
}

void LoginWindow::onShowSignup() {
    stackedWidget->setCurrentWidget(signupPage);
    lblSignupError->setVisible(false);
    leSignupUsername->clear();
    leSignupEmail->clear();
    leSignupPassword->clear();
    leSignupConfirmPassword->clear();
}

void LoginWindow::onShowLogin() {
    stackedWidget->setCurrentWidget(loginPage);
    lblLoginError->setVisible(false);
    leLoginUsername->clear();
    leLoginPassword->clear();
}

void LoginWindow::onForgotPassword() {
    QMessageBox::information(this, "Reset Password", "Please contact the system administrator to reset your password.");
}