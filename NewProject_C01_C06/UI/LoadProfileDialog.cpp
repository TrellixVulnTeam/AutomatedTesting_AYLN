#include "LoadProfileDialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QRegExp>
#include <QDebug>
#include "ConfigParse.h"

LoadProfileWidget::LoadProfileWidget(QWidget* parent) : QWidget(parent)
{
    m_folderPathLineEdit = new QLineEdit(this);
    m_folderPathLineEdit->setClearButtonEnabled(true);
    m_chooseFolderBtn = new QPushButton(tr("Choose Folder"), this);
    m_fileTableWidget = new QTableWidget(this);
    m_fileTableWidget->setColumnCount(2);
    QStringList header;
    header << tr("No") << tr("FileName");
    m_fileTableWidget->setHorizontalHeaderLabels(header);
    m_fileTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_fileTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fileTableWidget->verticalHeader()->setVisible(false);
    m_fileTableWidget->horizontalHeader()->setStretchLastSection(true);
    //    m_fileTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    connect(m_chooseFolderBtn, &QPushButton::clicked, this, &LoadProfileWidget::onChooseFolder);
    connect(m_fileTableWidget, &QTableWidget::itemClicked, this, &LoadProfileWidget::onFileTableWidgetItemClicked);
    //    connect(m_fileTableWidget, &QTableWidget::itemDoubleClicked, this,
    //            &LoadProfileWidget::onFileTableWidgetItemDoubleClicked);

    QHBoxLayout* topHlayout = new QHBoxLayout();
    topHlayout->addWidget(m_folderPathLineEdit);
    topHlayout->addWidget(m_chooseFolderBtn);

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addLayout(topHlayout);
    vlayout->addWidget(m_fileTableWidget);
    setLayout(vlayout);
}

LoadProfileWidget::~LoadProfileWidget() {}

void LoadProfileWidget::setProfile(const QString& fullPath, const QString& filterStr)
{
    QFileInfo fileInfo(fullPath);
    if (!fileInfo.exists()) {
    }
    m_profile = fullPath;
    loadCSVFilesFromFolder(fileInfo.path(), filterStr);
    selectCSVRow(fileInfo.baseName());
}

const QString& LoadProfileWidget::getProfile()
{
    return m_profile;
}

void LoadProfileWidget::loadCSVFilesFromFolder(const QString& folderPath, const QString& filterStr)
{
    if (!folderPathExist(folderPath)) {
        const QString errStr = QString("Csv file:%1 is not exist!").arg(folderPath);
        QMessageBox::critical(this, tr("Load csv"), errStr);
        return;
    }

    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(QStringList("*.csv"), QDir::Files, QDir::Name);
    QFileInfoList validFileList;
    if (!filterStr.isEmpty()) {
        foreach (QFileInfo fileinfo, fileList) {
            if (fileinfo.fileName().contains(filterStr, Qt::CaseInsensitive)) {
                validFileList.push_back(fileinfo);
            }
        }
    } else {
        validFileList = fileList;
    }

    m_folderPathLineEdit->setText(folderPath);
    for (int i = m_fileTableWidget->rowCount(); i > 0; i--) {
        m_fileTableWidget->removeRow(0);
    }

    for (int i = 0; i < validFileList.count(); i++) {
        QTableWidgetItem* noItem = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem* fileNameItem = new QTableWidgetItem(validFileList[i].fileName());
        noItem->setData(Qt::UserRole + 1, QVariant(validFileList[i].canonicalFilePath()));
        fileNameItem->setData(Qt::UserRole + 1, QVariant(validFileList[i].canonicalFilePath()));
        m_fileTableWidget->insertRow(m_fileTableWidget->rowCount());
        m_fileTableWidget->setItem(i, 0, noItem);
        m_fileTableWidget->setItem(i, 1, fileNameItem);
    }
}

bool LoadProfileWidget::folderPathExist(const QString& folderPath)
{
    QDir dir(folderPath);
    if (dir.exists()) {
        return true;
    }
    return false;
}

void LoadProfileWidget::selectCSVRow(const QString& baseName)
{
    QString str = baseName + ".csv";
    QList<QTableWidgetItem*> list = m_fileTableWidget->findItems(str, Qt::MatchExactly);
    for (int i = 0; i < list.size(); i++) {
        m_fileTableWidget->selectRow(list.value(i)->row());
    }
}

void LoadProfileWidget::onChooseFolder()
{

    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Choose CSV folder"),
                                                           QApplication::applicationDirPath() + tr("/TM/"));
    loadCSVFilesFromFolder(folderPath);
}

void LoadProfileWidget::onFileTableWidgetItemClicked(QTableWidgetItem* item)
{
    if (item) {
        m_profile = item->data(Qt::UserRole + 1).toString();
    }
}

void LoadProfileWidget::onFileTableWidgetItemDoubleClicked(QTableWidgetItem* item)
{
    if (item) {
        m_profile = item->data(Qt::UserRole + 1).toString();
        emit itemDoubleClicked();
    }
}

void LoadProfileWidget::clickLanguageAction()
{
    m_chooseFolderBtn->setText(tr("Choose Folder"));
    QStringList header;
    header << tr("No") << tr("FileName");
    m_fileTableWidget->setHorizontalHeaderLabels(header);
}

LoadProfileDialog::LoadProfileDialog(QWidget* parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setStyleSheet("QDialog{min-height:18em;min-width:30em;}");

    QTabWidget* tabWidget = new QTabWidget(this);
    LoadProfileWidget* flowWidget = new LoadProfileWidget(this);
    flowWidget->setProfile(QString::fromStdString(ConfigParse::getInstance().getFlowCsvPath()), "flow");
    tabWidget->addTab(flowWidget, tr("Flow"));
    connect(flowWidget, &LoadProfileWidget::itemDoubleClicked, this, &LoadProfileDialog::onLoad);
    connect(this, &LoadProfileDialog::clickLanguageActionSignal, [=]() { flowWidget->clickLanguageAction(); });

    LoadProfileWidget* specWidget = new LoadProfileWidget(this);
    specWidget->setProfile(QString::fromStdString(ConfigParse::getInstance().getSpecCsvPath()), "spec");
    tabWidget->addTab(specWidget, tr("Spec"));
    connect(specWidget, &LoadProfileWidget::itemDoubleClicked, this, &LoadProfileDialog::onLoad);
    connect(this, &LoadProfileDialog::clickLanguageActionSignal, [=]() { specWidget->clickLanguageAction(); });

    LoadProfileWidget* positionWidget = new LoadProfileWidget(this);
    positionWidget->setProfile(QString::fromStdString(ConfigParse::getInstance().getPositionCsvPath()), "position");
    tabWidget->addTab(positionWidget, tr("Position"));
    connect(positionWidget, &LoadProfileWidget::itemDoubleClicked, this, &LoadProfileDialog::onLoad);
    connect(this, &LoadProfileDialog::clickLanguageActionSignal, [=]() { positionWidget->clickLanguageAction(); });

    m_profileWidgetList.push_back(flowWidget);
    m_profileWidgetList.push_back(specWidget);
    m_profileWidgetList.push_back(positionWidget);

    m_loadBtn = new QPushButton(tr("Load"), this);
    m_cancel = new QPushButton(tr("Cancel"), this);
    connect(m_loadBtn, &QPushButton::clicked, this, &LoadProfileDialog::onLoad);
    connect(m_cancel, &QPushButton::clicked, this, &LoadProfileDialog::onCancel);

    QVBoxLayout* vlayout = new QVBoxLayout();
    QHBoxLayout* hBottomLayout = new QHBoxLayout();
    hBottomLayout->addWidget(m_loadBtn);
    hBottomLayout->insertStretch(1, QSizePolicy::Expanding);
    hBottomLayout->addWidget(m_cancel);

    vlayout->addWidget(tabWidget);
    vlayout->addLayout(hBottomLayout);
    setLayout(vlayout);
}

LoadProfileDialog::~LoadProfileDialog() {}

void LoadProfileDialog::onLoad()
{
    if (!checkProfileIsValid()) {
        return;
    }

    LoadProfileWidget* flowWidget = m_profileWidgetList.at(0);
    LoadProfileWidget* specWidget = m_profileWidgetList.at(1);
    LoadProfileWidget* positionWidget = m_profileWidgetList.at(2);

    ConfigParse::getInstance().setTestInfo(KFlowCsvPath, flowWidget->getProfile().toStdString());
    ConfigParse::getInstance().setTestInfo(KSpecCsvPath, specWidget->getProfile().toStdString());
    ConfigParse::getInstance().setTestInfo(KPositionCsvPath, positionWidget->getProfile().toStdString());

    emit loadCSVFileSuccessSignal();

    close();
}

void LoadProfileDialog::onCancel()
{
    close();
}

bool LoadProfileDialog::checkProfileIsValid()
{
    QString errorMsg;
    for (int i = 0; i < m_profileWidgetList.size(); i++) {
        const QString csvFile = m_profileWidgetList.value(i)->getProfile();
        QFile file(csvFile);
        if (!file.exists()) {
            errorMsg += QString(tr("Not find csv file ,Path:%2 ")).arg(m_profileWidgetList.value(i)->getProfile());
        }
    }

    if (!errorMsg.isEmpty() || !errorMsg.isNull()) {
        QMessageBox::warning(this, tr("Load csv"), errorMsg);
        return false;
    }
    return true;
}

void LoadProfileDialog::clickLanguageAction(QTranslator&)
{
    m_loadBtn->setText(tr("Load"));
    m_cancel->setText(tr("Cancel"));
    emit clickLanguageActionSignal();
}
