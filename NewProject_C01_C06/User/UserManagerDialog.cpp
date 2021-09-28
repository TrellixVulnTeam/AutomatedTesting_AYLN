#include "UserManagerDialog.h"
#include "ui_UserManagerDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QValidator>

UserManagerDialog::UserManagerDialog(QWidget* parent) : QDialog(parent), ui(new Ui::UserManagerDialog)
{
    ui->setupUi(this);
    setStyleSheet("QDialog{min-height:23em;min-width:20em;}");
#if defined(Q_OS_WIN)
    QString path = QApplication::applicationDirPath() + "/Config/UserInfo.ini";
    m_userSetting = new QSettings(path, QSettings::IniFormat);
#elif defined(Q_OS_MAC)
    QString path = QApplication::applicationDirPath().replace("MacOS", "Resources/UserInfo.plist");
    m_userSetting = new QSettings(path, QSettings::NativeFormat);
#endif
    m_table = ui->userList;
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setColumnWidth(0, 20);
    m_table->setColumnWidth(0, 60);
    m_table->setColumnWidth(0, 60);
    QRegExp rxNoChinese("[^\u4e00-\u9fa5,]*$");
    QValidator* validator = new QRegExpValidator(rxNoChinese);
    ui->add_userNameLineEdit->setValidator(validator);
    ui->add_passwordLineEdit->setValidator(validator);
    ui->add_confirmLineEdit->setValidator(validator);
    ui->modify_passwordLineEdit->setValidator(validator);
    ui->modify_confirmLineEdit->setValidator(validator);

    initUserList();
    onUpdateModifyWidgetUserNameCombobox(m_userSetting);
    connect(ui->addBtn, &QPushButton::clicked, this, &UserManagerDialog::onAdd);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &UserManagerDialog::onModify);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &UserManagerDialog::onDeleteUser);

    connect(ui->add_cancelBtn, &QPushButton::clicked, this, &UserManagerDialog::onCancel);
    connect(ui->add_doneBtn, &QPushButton::clicked, this, &UserManagerDialog::onAddDone);

    connect(ui->modify_cancelBtn, &QPushButton::clicked, this, &UserManagerDialog::onCancel);
    connect(ui->modify_doneBtn, &QPushButton::clicked, this, &UserManagerDialog::onModifyDone);

    connect(ui->modify_userNameComboBox,
            static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this,
            &UserManagerDialog::onUpdateModifyWidgetOperatorCombobox);
    connect(this, &UserManagerDialog::userInfoChanged, this, &UserManagerDialog::onUpdateModifyWidgetUserNameCombobox);
    onCancel();
    m_table->clearFocus();
}

void UserManagerDialog::initUserList()
{
    QStringList userNameList = m_userSetting->allKeys();
    if (!userNameList.contains(TESTE)) {
        User* user = new User();
        user->setName(TESTE);
        user->setPasswd(DEFAULT_TESTE_PW);
        user->setAuthority(User::TestE);
        user->serialize();
        m_userSetting->setValue(TESTE, user->getUserInfo());
        delete user;
        user = NULL;
        userNameList = m_userSetting->allKeys();
    }
    if (!userNameList.contains(OPERATOR)) {
        User* user = new User();
        user->setName(OPERATOR);
        user->setPasswd(DEFAULT_PW);
        user->setAuthority(User::Operator);
        user->serialize();
        m_userSetting->setValue(OPERATOR, user->getUserInfo());
        delete user;
        user = NULL;
        userNameList = m_userSetting->allKeys();
    }
    if (!userNameList.contains(DEVELOPER)) {
        User* user = new User();
        user->setName(DEVELOPER);
        user->setPasswd(DEFAULT_DEVELOPER_PW);
        user->setAuthority(User::Developer);
        user->serialize();
        m_userSetting->setValue(DEVELOPER, user->getUserInfo());
        delete user;
        user = NULL;
        userNameList = m_userSetting->allKeys();
    }

    m_table->setRowCount(userNameList.size());
    m_table->setColumnCount(3);
    m_table->verticalHeader()->setHidden(true);
    for (int i = 0; i < userNameList.size(); ++i) {
        QString name = userNameList.at(i);
        m_userInfoMap[name] = m_userSetting->value(name);
        QVariantMap userAuthority = m_userInfoMap[name].toMap();

        m_table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        m_table->setItem(i, 1, new QTableWidgetItem(userNameList.at(i)));

        switch (userAuthority.value(AUTHORITY).toInt()) {
        case User::Operator:
            m_table->setItem(i, 2, new QTableWidgetItem(OPERATOR));
            break;
        case User::Developer:
            m_table->setItem(i, 2, new QTableWidgetItem(DEVELOPER));
            break;
        case User::TestE:
            m_table->setItem(i, 2, new QTableWidgetItem(TESTE));
            break;
        }
    }
}

void UserManagerDialog::userInfoManager(UserOperat operat)
{
    User* user = new User();
    int rowIndex = -1;
    user->setAuthority(getUserAuthority(operat));
    if (operat == ModifyUser) {
        QList<QTableWidgetItem*> itemList =
            m_table->findItems(ui->modify_userNameComboBox->currentText(), Qt::MatchFixedString);
        rowIndex = itemList.at(0)->row();
        user->setName(ui->modify_userNameComboBox->currentText());
        user->setPasswd(ui->modify_passwordLineEdit->text());
        ui->modify_passwordLineEdit->clear();
        ui->modify_confirmLineEdit->clear();
        switch (user->getAuthority()) {
        case 0:
            m_table->item(rowIndex, 2)->setText("OP");
            break;
        case 1:
            m_table->item(rowIndex, 2)->setText("Engineer");
            break;
        case 2:
            m_table->item(rowIndex, 2)->setText("TE");
            break;
        }
    } else {
        rowIndex = m_table->rowCount();
        user->setName(ui->add_userNameLineEdit->text());
        user->setPasswd(ui->add_passwordLineEdit->text());
        m_table->insertRow(rowIndex);
        m_table->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(rowIndex)));
        m_table->setItem(rowIndex, 1, new QTableWidgetItem(user->getName()));
        switch (user->getAuthority()) {
        case 0:
            m_table->setItem(rowIndex, 2, new QTableWidgetItem(OPERATOR));
            break;
        case 1:
            m_table->setItem(rowIndex, 2, new QTableWidgetItem(DEVELOPER));
            break;
        case 2:
            m_table->setItem(rowIndex, 2, new QTableWidgetItem(TESTE));
            break;
        }
        ui->add_confirmLineEdit->clear();
        ui->add_passwordLineEdit->clear();
        ui->add_userNameLineEdit->clear();
    }
    user->serialize();
    QVariantMap userInfo = user->getUserInfo();
    m_userInfoMap.insert(user->getName(), userInfo);
    m_userSetting->setValue(user->getName(), userInfo);
    delete user;
    user = nullptr;
    onCancel();
    emit userInfoChanged(m_userSetting);
}

void UserManagerDialog::onUpdateModifyWidgetOperatorCombobox(const QString& userName)
{
    if (userName == OPERATOR || userName == DEVELOPER) {
        ui->modify_authorityBox->setCurrentText(userName);
        ui->modify_authorityBox->setEnabled(false);
    } else {
        ui->modify_authorityBox->setEnabled(true);
    }
}

void UserManagerDialog::onDeleteUser()
{
    QList<QTableWidgetSelectionRange> ranges = m_table->selectedRanges();
    if (ranges.size() == 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select the users before you push the delete button !"));
    } else {
        if (QMessageBox::Yes
            == QMessageBox::question(this, tr("Warning"), tr("Are you sure to delete this/these user infomation?"),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)) {
            QList<int> rowIndex;
            int count = ranges.count();
            for (int i = 0; i < count; ++i) {
                int topRow = ranges.at(i).topRow();
                int bottomRow = ranges.at(i).bottomRow();
                for (int j = topRow; j <= bottomRow; ++j) {
                    rowIndex << j;
                }
            }
            qSort(rowIndex.begin(), rowIndex.end());

            for (int i = 1; i <= count; ++i) {
                QTableWidgetItem* item = m_table->item(rowIndex.at(count - i), 1);
                QString name = item->text();
                if (isDefaultUser(name)) {
                    continue;
                }
                m_table->removeRow(rowIndex.at(count - i));
                m_userInfoMap.remove(name);
                m_userSetting->remove(name);
                item = nullptr;
            }
            sortItems();
            emit userInfoChanged(m_userSetting);
        }
    }
}

void UserManagerDialog::onAdd()
{
    int addWidgetIndex = ui->stackedWidget->indexOf(ui->page_add);
    ui->stackedWidget->setCurrentIndex(addWidgetIndex);
    ui->add_userNameLineEdit->setFocus();
}
void UserManagerDialog::onModify()
{
    int modifyWidgetIndex = ui->stackedWidget->indexOf(ui->page_modify);
    ui->stackedWidget->setCurrentIndex(modifyWidgetIndex);
    ui->modify_passwordLineEdit->setFocus();
}

void UserManagerDialog::onAddDone()
{
    if (ui->add_userNameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please input user name !"));
        return;
    }
    if (isDefaultUser(ui->add_userNameLineEdit->text())) {
        return;
    }
    if (m_userInfoMap.contains(ui->add_userNameLineEdit->text())) {
        QMessageBox::warning(this, tr("Warning"), tr("User already exists, don't allow to add !"));
        return;
    }
    if (ui->add_passwordLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please input password !"));
        return;
    }
    if (ui->add_passwordLineEdit->text() != ui->add_confirmLineEdit->text()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("The password and confirmation do not match. Please re-enter the user infomation !"));
        ui->add_passwordLineEdit->clear();
        ui->add_confirmLineEdit->clear();
        return;
    }
    userInfoManager(AddUser);
}

void UserManagerDialog::onModifyDone()
{
    if (ui->modify_userNameComboBox->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Select the user you want to modify!"));
        return;
    }
    if (ui->modify_passwordLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please input password !"));
        return;
    }
    if (ui->modify_passwordLineEdit->text() != ui->modify_confirmLineEdit->text()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("The password and confirmation do not match. Please re-enter the user infomation !"));
        ui->modify_passwordLineEdit->clear();
        ui->modify_confirmLineEdit->clear();
        return;
    }
    userInfoManager(ModifyUser);
}

void UserManagerDialog::onCancel()
{
    int mainWidgetIndex = ui->stackedWidget->indexOf(ui->page);
    ui->stackedWidget->setCurrentIndex(mainWidgetIndex);
}

void UserManagerDialog::onUpdateModifyWidgetUserNameCombobox(QSettings* setting)
{
    Q_UNUSED(setting);
    ui->modify_userNameComboBox->clear();
    for (auto it = m_userInfoMap.begin(); it != m_userInfoMap.end(); it++) {
        if (it.key() == TESTE) {
            continue;
        }
        ui->modify_userNameComboBox->addItem(it.key());
    }
}

void UserManagerDialog::sortItems()
{
    int count = m_table->rowCount();
    for (int i = 0; i < count; ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
    }
}

User::Authority UserManagerDialog::getUserAuthority(UserOperat operat)
{
    QString authority;
    if (operat == AddUser) {
        authority = ui->add_authorityBox->currentText();
    } else {
        authority = ui->modify_authorityBox->currentText();
    }

    if (authority == TESTE) {
        return User::TestE;
    } else if (authority == OPERATOR) {
        return User::Operator;
    } else if (authority == DEVELOPER) {
        return User::Developer;
    } else {
        return User::Invalid;
    }
}

bool UserManagerDialog::isDefaultUser(const QString& userName)
{
    if (userName == TESTE || userName == OPERATOR || userName == DEVELOPER) {
        QMessageBox::warning(this, tr("Warning"), tr("You can't operate the default user !"));
        return true;
    } else {
        return false;
    }
}

void UserManagerDialog::clickLanguageAction(QTranslator& tran)
{
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);
}
