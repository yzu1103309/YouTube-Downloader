# YouTube-Downloader
Download YouTube videos and audio with ease. GUI for yt-dlp written in Qt C++.
![Screenshot_20240301_151557](https://github.com/yzu1103309/YouTube-Downloader/assets/97399678/e2a50722-bd3f-432e-9df0-57fef877f570)

## Main Features

This program runs [yt-dlp](https://github.com/yt-dlp/yt-dlp) in backend (already packed in the release packages)

You can easily adjust some basic settings with our GUI, such as Download mode (single video or playlist), video or audio, quality, and whether to download the subtitles

Besides YouTube, you can also try to download from other websites (yt-dlp supports loads of sites).

(For some sites that isn't supported natively, you can find the m3u8 url of the video and paste it.)

The program will organize the downloaded files in a pleasant structure

## 2024/03/01 Updates (Version 3.0.0)

[+] Added codec selection menu (VP9, AV1, H.264).

[+] Enhanced usage flexibility with the ability to generate and copy command to clipboard.

[+] Download path saved upon exit, eliminating the need for manual selection each time.

[Major (Ubuntu)] yt-dlp will be installed to '/usr/local/bin' instead of '/usr/bin' to prevent package conflicts.

[Update] yt-dlp version updated to 2023.12.30.

[Deprecate] Dropped support for Ubuntu 20.04 Focal.

[Fixes] YouTube sharing link 'yout.be' is now working correctly.

## Installation

Support Ubuntu (>=22.04) and Windows (both tested)

Download .deb or .exe from release tab and install.

## ⚠️ Troubleshooting

### Error Occurs While Downloading ⛔

If you encounter a "403 Forbidden" error (or other errors) while downloading with the latest release, it might be because the bundled version of yt-dlp is outdated.

To resolve this, open a terminal with administrator privileges and run the command:
```
yt-dlp --update
```
This will update the backend program.

(Note: The frontend UI is functioning correctly, so I will not release a new version unless a future yt-dlp update causes incompatibility with it.)

### Windows Defender SmartScreen 🚨

In Windows, the setup.exe might be considered as unsafe by Windows Defender SmartScreen (Which is quite annoying ...)

And in some cases, there isn't the "Run Anyway" Option.

If so, you can bypass the warning by launching the setup manually via the command line:

1. Open `cmd` or `powershell`
2. Type in the absolute path of the setup.exe file

The installation program should start without warning.
