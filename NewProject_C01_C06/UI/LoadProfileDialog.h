#ifndef LOADPROFILEDIALOG_H
#define LOADPROFILEDIALOG_H
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QAction>
#include <QLayout>
#include <QFileDialog>
#include <QDir>
#include <QLabel>
#include <QComboBox>
#include <QTranslator>

class LoadProfileWidget : public QWidget
{
    Q_OBJECT

public:
    LoadProfileWidget(QWidget* parent = 0);
    ~LoadProfileWidget();
    void setProfile(const QString& fullPath, const QString& filterStr = "");
    const QString& getProfile();

public slots:
    void clickLanguageAction();

signals:
    void itemDoubleClicked();

private:
    QString m_profile;
    QLineEdit* m_folderPathLineEdit = NULL;
    QPushButton* m_chooseFolderBtn = NULL;
    QTableWidget* m_fileTableWidget = NULL;

private:
    void loadCSVFilesFromFolder(const QString& folderPath, const QString& filterStr = "");
    bool folderPathExist(const QString& folderPath);
    QFileInfoList csvFilesFilter(QFileInfoList fileList);
    void selectCSVRow(const QString& baseName);

private slots:
    void onFileTableWidgetItemClicked(QTableWidgetItem* item);
    void onFileTableWidgetItemDoubleClicked(QTableWidgetItem* item);
    void onChooseFolder();
};

class LoadProfileDialog : public QDialog
{
    Q_OBJECT

public:
    LoadProfileDialog(QWidget* parent = 0);
    ~LoadProfileDialog();

public slots:
    void clickLanguageAction(QTranslator&);

private:
    void showEvent(QShowEvent* event) override;

private:
    QPushButton* m_loadBtn = NULL;
    QPushButton* m_cancel = NULL;
    QList<LoadProfileWidget*> m_profileWidgetList;
    bool checkProfileIsValid();

signals:
    void loadCSVFileSuccessSignal();
    void clickLanguageActionSignal();

private slots:
    void onLoad();
    void onCancel();
};

#endif  // LOADPROFILEDIALOG_H
