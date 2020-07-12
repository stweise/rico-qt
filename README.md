# rico-qt

rico-qt is a QT based applications that allows simple and fast creation of rico clusters [1,2]. It uses a simple (example: [rico-qt-save.json](resources/rico-qt-save.json) ) JSON representation to store documents. It also generates small efficient PDF output (example: [rico-qt-print.pdf](resources/rico-qt-print.pdf) ) you can easily use to print, present and postprocess.

[1] https://de.wikipedia.org/wiki/Cluster_(Kreatives_Schreiben)
[2] https://www.lifehack.org/articles/featured/rico-clusters-an-alternative-to-mind-mapping.html
## Screenshots

![Main Application Window](resources/rico-qt.png)
![Main Menu](resources/rico-qt-2.png)

## Installation

### Installation on Mac OS X

Open the released DMG file for Mac OS X and simple copy the included Application in your "Applications" folder.

### Installation on Windows

Unzip the relased 7z file using an unpacker. If you do not have one, please go to [z-zip.org](https://www.7-zip.org/) and download and install it on your Windows. If you get any error with regard to a missing MSVCP140.dll please also install the vcredist_x64.exe which is already in the folder. This is taken from [here](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).

### Installation from source

Either compile it using the QT open source license or download one of the releases.

In QT Creator:
Project > Open Project > Import the CMakeLists.txt > Build


## Usage

After starting the application use the following input commands:

* double-click on empty space: create a new node
* double-click on a node: update node label
* single-click on a node: selects node
* (Windows & Linux) CTRL-click on a node or edge: toggles selection on node or edge
* (Mac) COMMAND-click on a node or edge: toggles selection on node or edge
* "e"-key + two selected nodes: nodes are connected by an edge 
* DELETE/BACKSPACE: deletes edge/node under current selection
* drag and drop is supported (extendable by selection)

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)
2020 Steffen Weise

## Notice

This project uses the following tools/libraries:

* Qt Open Source Edition [QT5.15](https://qt.io)
** please also see: [license details QT](https://doc.qt.io/qt-5/licenses-used-in-qt.html)
* Microsoft Visual Studio 2019 [MSVS2019](https://visualstudio.microsoft.com/)
* Apple Xcode [Xcode](https://developer.apple.com/xcode/)
* Fedora 31 [Fed31](https://getfedora.org)