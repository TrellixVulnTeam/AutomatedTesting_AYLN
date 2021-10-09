#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDate>
#include <QTime>
#include <cerrno>
#include "LogHelper.h"
#include "ConfigParse.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    setStyleSheet("QDialog{min-height:10em;min-width:31em;max-height:10em;max-width:31em;}");
#else
    setStyleSheet("QDialog{min-height:10em;min-width:34em;max-height:10em;max-width:34em;}");
#endif
    connect(ui->urlLab, &QLabel::linkActivated, this, &AboutDialog::openUrl);

    // get build time
    const QDate buildDate = QLocale(QLocale::English).toDate(QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    const QTime buildTime = QTime::fromString(__TIME__, "hh:mm:ss");
    const QString time_build = QString("%1 %2").arg(buildDate.toString("yyyy.MM.dd")).arg(buildTime.toString());
    ui->buildTime->setText(QString("%1 %2").arg(tr("Built on ")).arg(time_build));

    ui->softwareVersion->setText(QString::fromStdString(CFG_PARSE.getVersion()));
    ui->qtVersion->setText(buildCompatibilityString());
}

QString AboutDialog::compilerString()
{
#if defined(Q_CC_CLANG)  // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__)  // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang ") + QString::number(__clang_major__) + QLatin1Char('.')
           + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC ") + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER > 1999)
        return QLatin1String("MSVC <unknown>");
    if (_MSC_VER >= 1910)
        return QLatin1String("MSVC 2017");
    if (_MSC_VER >= 1900)
        return QLatin1String("MSVC 2015");
#endif
    return QLatin1String("<unknown compiler>");
}

QString AboutDialog::buildCompatibilityString()
{
    return tr("Based on Qt %1 (%2, %3 bit)")
        .arg(QLatin1String(qVersion()), compilerString(), QString::number(QSysInfo::WordSize));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::openUrl(const QString& url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void AboutDialog::onClickLanguageAction(QTranslator& tran)
{
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);
}
