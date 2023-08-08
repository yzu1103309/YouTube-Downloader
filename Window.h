#ifndef QT_PROJECT_WINDOW_H
#define QT_PROJECT_WINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QButtonGroup>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QProcess>
#include <string>
using namespace std;

class Window: public QWidget
{
    Q_OBJECT
public:
    Window();

private:
    QGroupBox *url_area;
    QGroupBox *mode_area;
    QGroupBox *opt_area;
    QGroupBox *path_area;
    QGroupBox *status_area;

    QLineEdit *url;
    QButtonGroup *playlist_grp;
    QRadioButton *single;
    QRadioButton *multiple;
    QButtonGroup *type_grp;
    QRadioButton *video;
    QRadioButton *audio;
    QComboBox *quality;
    QCheckBox *subs;

    QLineEdit *path;
    QPushButton *choosePath;
    QProcess *downloader = new QProcess;

    QPushButton *download;
    QPushButton *stop;

    QLabel *status;
    QProgressBar *progress;

    string command;
    char percent[3];

    void initLayout();
    void initButtons();

    void showVidOpt()
    {
        quality->clear();
        quality->addItem("Best Video");
        quality->addItem("1080p");
        quality->addItem("720p");
        quality->addItem("480p");
    }
    void showAudOpt()
    {
        quality->clear();
        quality->addItem("Best Audio");
        quality->addItem("320k");
        quality->addItem("256k");
        quality->addItem("192k");
    }
    void chooseDir()
    {
        QString dir = QFileDialog::getExistingDirectory(nullptr, QString(), path->text());
        if(!dir.isEmpty())
        {
            path->setText(dir);
        }
    }
    void startDownload();
    void stopJob();
private slots:
    void getOutput();
    void getError();
    void checkStatus();
};

#endif //QT_PROJECT_WINDOW_H
