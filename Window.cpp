#include <QLayout>
#include <QFormLayout>
#include <QScreen>
#include "Window.h"

int fixedW = 1000;
int fixedH = 550;

Window::Window()
{
    setWindowTitle("YouTube Downloader");
    setWindowIcon(QIcon("./youtube.png"));
    setFixedWidth(fixedW);
    setFixedHeight(fixedH);
    move(screen()->geometry().center() - frameGeometry().center());
    initLayout();

    download = new QPushButton("Download");
    download->setFixedWidth(150);
    download->setFixedHeight(40);
    connect(download, &QPushButton::clicked, this, &Window::startDownload);

    stop = new QPushButton("Stop");
    stop->setFixedWidth(150);
    stop->setFixedHeight(40);
    connect(stop, &QPushButton::clicked, this, &Window::stopJob);

    QHBoxLayout *action_area = new QHBoxLayout();
    action_area->addStretch();
    action_area->addWidget(download, 0, Qt::AlignCenter);
    action_area->addSpacerItem(new QSpacerItem(20, 0));
    action_area->addWidget(stop, 0, Qt::AlignCenter);
    action_area->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(url_area);
    mainLayout->addWidget(mode_area);
    mainLayout->addWidget(opt_area);
    mainLayout->addWidget(path_area);
    mainLayout->addSpacerItem(new QSpacerItem(0,25));
    mainLayout->addLayout(action_area);
    mainLayout->addSpacerItem(new QSpacerItem(0,25));
    mainLayout->addWidget(status_area);
}

void Window::initLayout()
{
    url_area = new QGroupBox;
    url = new QLineEdit;
    QFormLayout *layout1 = new QFormLayout;
    layout1->addRow(new QLabel("URL: "), url);
    url_area->setLayout(layout1);
    url_area->setFixedHeight(50);

    mode_area = new QGroupBox;
    QHBoxLayout *modeLayout = new QHBoxLayout;
    playlist_grp = new QButtonGroup;
    single = new QRadioButton("Single Video");
    multiple = new QRadioButton("Whole Playlist");
    playlist_grp->addButton(single);
    playlist_grp->addButton(multiple);
    modeLayout->addWidget(new QLabel("Mode Selection: "), 0, Qt::AlignCenter);
    modeLayout->addWidget(single, 0, Qt::AlignCenter);
    modeLayout->addWidget(multiple, 0, Qt::AlignCenter);
    single->setChecked(true);
    mode_area->setLayout(modeLayout);

    opt_area = new QGroupBox;
    quality = new QComboBox;
    initButtons();
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(new QLabel("Options: "));
    layout2->addWidget(video);
    layout2->addWidget(audio);
    layout2->addWidget(quality);
    layout2->addWidget(subs, 0,Qt::AlignRight);
    opt_area->setLayout(layout2);

    path_area = new QGroupBox;
    path = new QLineEdit;
    path->setReadOnly(true);
    path->setText("$HOME/Downloads/YouTube Downloader");
    choosePath = new QPushButton("Choose");
    choosePath->setIcon(QIcon::fromTheme("inode-directory"));
    connect(choosePath, &QPushButton::clicked, this, &Window::chooseDir);
    QGridLayout *layout3 = new QGridLayout;
    layout3->addWidget(new QLabel("Path: "), 0,0);
    layout3->addWidget(path, 0, 1);
    layout3->addWidget(choosePath, 0, 2);
    path_area->setLayout(layout3);

    status_area = new QGroupBox;
    status = new QLabel("Status");
    //status->setFixedWidth(fixedW - 30);
    status->setFixedHeight(80);
    status->setAlignment(Qt::AlignCenter);
    status->setWordWrap(true);
    progress = new QProgressBar;
    progress->setValue(0);
    QGridLayout *layout4 = new QGridLayout;
    layout4->addWidget(status, 1,0);
    layout4->addWidget(progress, 2,0);
    status_area->setLayout(layout4);
}

void Window::initButtons()
{
    type_grp = new QButtonGroup;
    video = new QRadioButton("Video (mp4)");
    audio = new QRadioButton("Audio (mp3)");
    subs = new QCheckBox("Subtitles (all)");

    connect(video, &QRadioButton::clicked, this, &Window::showVidOpt);
    connect(audio, &QRadioButton::clicked, this, &Window::showAudOpt);

    video->setChecked(true);
    showVidOpt();

    type_grp->addButton(video);
    type_grp->addButton(audio);
}

void Window::startDownload()
{
    if(downloader->state() == QProcess::NotRunning)
    {
        percent[0] = '0';
        percent[1] = '0';
        percent[1] = '0';
        progress->setValue(0);
        QString url_text = url->text();
        QString dir = path->text();
        size_t youtube_url = url_text.toStdString().find("youtube");
        if(url_text.isEmpty())
        {
            status->setText("[ERROR] URL Required");
            return;
        }
        if(dir.isEmpty())
        {
            status->setText("[ERROR] Choose a directory to store the file");
            return;
        }
        if(single->isChecked() && youtube_url != string::npos)
        {
            size_t watch_found = url_text.toStdString().find("watch");
            size_t short_found = url_text.toStdString().find("short");
            if(watch_found == string::npos && short_found == string::npos)
            {
                status->setText("[ERROR] This url doesn't seem to contain any video.\nMaybe it's a playlist? Change to playlist mode and retry.");
                return;
            }
        }
        if(multiple->isChecked())
        {
            size_t found = url_text.toStdString().find("list");
            if(found == string::npos)
            {
                status->setText("[ERROR] This url doesn't seem to be a playlist.\nMaybe it's a video? Change to video mode and retry.");
                return;
            }
        }
        status->setText("Start downloading");
        repaint();

        QStringList args = (
                QStringList() << url_text.toStdString().c_str()
                << "--paths" << dir.toStdString().c_str() << "--no-color"
        );
        string kbps;
        args << "--format";
        if(video->isChecked())
        {
            if(youtube_url != string::npos)
            {
                switch (quality->currentIndex()) {
                    case 0: // best video
                        args << "bv[ext=mp4]+ba[ext=m4a]";
                        break;
                    case 1:
                        args << "bv[ext=mp4][height=1080]+ba[ext=m4a]";
                        break;
                    case 2:
                        args << "bv[ext=mp4][height=720]+ba[ext=m4a]";
                        break;
                    case 3:
                        args << "bv[ext=mp4][height=480]+ba[ext=m4a]";
                }
            }
            else
            {
                args << "bv*+ba*";
            }
        }
        else
        {
            if(youtube_url != string::npos)
            {
                args << "ba[ext=m4a]" << "-x" << "--audio-format" << "mp3" << "--audio-quality";
            }
            else
            {
                args << "ba*" << "-x" << "--audio-format" << "mp3" << "--audio-quality";
            }
            switch (quality->currentIndex()) {
                case 0: // best audio
                    kbps = "320k";
                    break;
                case 1:
                    kbps = "320k";
                    break;
                case 2:
                    kbps = "256k";
                    break;
                case 3:
                    kbps = "192k";
            }
            args << kbps.c_str();
        }

        if(video->isChecked() && subs->isChecked())
            args << "--write-subs" << "--sub-langs" << "all" << "--convert-subs" << "srt";

        if(single->isChecked())
            args << "--no-playlist";
        else
            args << "--yes-playlist";

        string o_template;
        if(multiple->isChecked())
        {
            if(video->isChecked())
                o_template = "[Playlist] %(playlist)s/%(playlist_index)s - %(title)s/%(playlist_index)s - %(title)s [%(height)sp].%(ext)s";
            else
                o_template = "[Playlist] %(playlist)s/%(playlist_index)s - %(title)s ["+kbps+"].%(ext)s";
        }
        else
        {
            if(video->isChecked())
                o_template = "%(title)s/%(title)s [%(height)sp].%(ext)s";
            else
                o_template = "%(title)s ["+kbps+"].%(ext)s";
        }
        args << "-o" << o_template.c_str();

        //qDebug() << args;
        delete downloader;
        downloader = new QProcess;
        downloader->start("yt-dlp", args);

        connect(downloader, SIGNAL(readyReadStandardOutput()),this, SLOT(getOutput()));
        connect(downloader, SIGNAL(readyReadStandardError()),this, SLOT(getError()));
        connect(downloader, SIGNAL(finished(int,QProcess::ExitStatus)),this, SLOT(checkStatus()));
    }
}

void Window::getOutput()
{
    QString output = QString::fromLocal8Bit(downloader->readAllStandardOutput());
    string tmp = output.toStdString();
    size_t found = tmp.find("[download]");
    if(found != string::npos) //found
    {
        tmp.erase(0, found+11);
    }
    status->setText(tmp.c_str());

    if(tmp[5] == '%')
    {
        tmp.copy(percent, 3, 0);
        progress->setValue( atoi(percent) );
    }
}
void Window::getError()
{
    QString output = downloader->readAllStandardError();
    status->setText(output);
}
void Window::checkStatus()
{
    if(downloader->exitStatus() == QProcess::NormalExit)
    {
        if( atoi(percent) == 100 )
        {
            status->setText("job finished!");
            repaint();
        }
        else
        {
            progress->setValue(100);
        }
    }
}
void Window::stopJob()
{
    if(downloader->state() == QProcess::Running)
    {
        downloader->kill();
        status->setText("Job canceled by user.");
        progress->setValue(0);
    }
}
