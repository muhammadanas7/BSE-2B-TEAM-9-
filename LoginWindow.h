#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include "UserAuth.h"
#include "FileManager.h"

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccessful(const QString& username, const QString& role);

private slots:
    void onLogin();
    void onShowSignup();
    void onShowLogin();
    void onSignup();
    void onForgotPassword();

private:
    QStackedWidget* stackedWidget;
    QWidget* loginPage;
    QLineEdit* leLoginUsername;
    QLineEdit* leLoginPassword;
    QPushButton* btnLogin;
    QPushButton* btnGoToSignup;
    QLabel* lblLoginError;

    QWidget* signupPage;
    QLineEdit* leSignupUsername;
    QLineEdit* leSignupEmail;
    QLineEdit* leSignupPassword;
    QLineEdit* leSignupConfirmPassword;
    QComboBox* cbRole;
    QPushButton* btnSignup;
    QPushButton* btnBackToLogin;
    QLabel* lblSignupError;

    FileManager* fileManager;

    void setupLoginPage();
    void setupSignupPage();
    bool validateEmail(const QString& email);
    bool usernameExists(const QString& username);
    bool emailExists(const QString& email);
    void saveUser(const UserAuth& user);
    bool authenticateUser(const QString& username, const QString& password, UserAuth& outUser);
    QString styleSheet();
};

#endif