# Prerequisites

This repository has been tested with the following setups.

**Win**

- Windows 8.1
- Visual Studio 2015
- Qt 5.8

**Mac**

- Mac 10.12.1, 10.11.6
- Clang 800.0.42.1
- Qt 5.7

**General**

- Make sure you can access the *QtoF_experiments* repository
- Make sure you can access the *openFrameworks* repository

# Installing

While developing this library I've used a shell script that I run in a
terminal on Mac or in a Git Bash console on windows. You can either
use this approach or use Qt Creator for development. Though, you have
to run `./release.sh` at least once to setup your project
environment. `release.sh` will setup openFrameworks for you. To make
this work cross platform, you have to create an environment variable
(on both Win and Mac).

**1. Create QT_PATH environment variable**

- On Win e.g.something like: `C:\Qt\5.8\msvc2015_64\`
- On Mac, edit your .bash_profile: `export QT_PATH="/Applications/Qt/5.7/clang_64/`

**2. Install the latest CMake**

**3. Setup development environment**

````sh
- Open Git Bash on Win or a terminal on Mac
# git clone git@github.com:SimileSystems/QtoF_experiments.git
# cd QtoF_experiments
# cd build
# ./release.sh
````

# Qt Creator

**1. Change default build directory:**

- Open _Options / Preferences_
- Select _Build & Run_
- Change _Default build directory_, remove the `..` from the _Default build directory_

**2. Setup CMake:**

- Install cmake and make sure you can run in a terminal.
- Open _Options / Preferences_
- Select _Build & Run_ > _CMake_
- When there is no _Auto-detected_ CMake entry, press _Add_ and select the path where CMake is installed.
I only had to do this on Mac.

**3. Configure the CMake Kit**

- Open _Options / Preferences_
- Select _Build & Run_ > _Kits_
- Click _Add_, name it _CMake_
- Click _Change..._ for the _CMake Generator_ (at the bottom).
    - **Mac:**
      - Generator: _Unix Makefiles_
      - Extra generator: _CodeBlocks_
    - **Win: (defaults)**
      - Generator: _NMake MakeFiles_
      - Extra generator: _CodeBlocks_
      - Compiler C: _Microsoft Visual C++ Compiler 14.0 (xxx64)_
      - Compiler C++_Microsoft Visual C++ Compiler 14.0 (xxx64)_
      - Debugger: _Auto-detected CDB at ... \x64\cdb.exe_

## Opening a CMake project in QtCreator

- Open Qt Creator
- _File_ > _Open File or Project_
- Select your CMakeLists.txt file (e.g. `apps/empty/build/CMakeLists.txt`)
- Select _[x] CMake_, then press the _Configure Project_ button.
- Open the `CMakeLists.txt` file and change the _project()_ value.
- _Build_ > _Build All_ 

# Creating a new Widget

_Scroll down if you want to skip the background and get to the steps
to create a widget._A widget is a piece of code that you write that
implements some feature that you want to use in Qt. It typically has a
`setup()`, `update()` and a `draw()` function. For this project we
implement a widget using openFrameworks, but it can be anything you
would like as long as it implements these (and a couple more)
functions. We decided to decouple the widget implementation from Qt
using opaque pointers (PIMPL idom).  One of the reasons why we did
this was to overcome the issues with openGL includes and overcoming
linker issues. Also this approach forces us to make sure our widgets
could potentially be used in other software. This does mean that for
every widget you need to create a thiny pimpl wrapper. I created some
javascript which does most of the work for you. The file
`experimental/CodeGenerator.js` generates a widget class and wrapper
around it for you. For now you have to open the file and edit the
classname.


        +---------+     +-----------+     +------+
        |         |     |           |     |      |
        | Widget  o---->|   PIMPL   o---->|  Qt  |
        |  (of)   |     | (wrapper) |     |      | 
        |         |     |           |     |      |
        +---------+     +-----------+     +------+


Once you have edited the `experimental/CodeGenerator.js` file you 
open a terminal an run `cd experimental && node CodeGenerator.js`.
Which will generate four files (two pairs): a piml wrapper and
some code that implements the basics of a widget. 

Before you can use the widget you have to do a bit of more work.  We
have to define the widget type in the `qtof/WidgetTypes.h` file.  Make
sure that you don't use a number as the first character as we use the
type to generatea an enum with the same name; and they are not allowed
to start with a number. So open this file and add your type. After
that open a terminal and execute our *second* different code generator
in `qtof/CodeGenerator.js` (_not that this is a different code
generator as in the previous paragraphs._). This second code generator
creates some some that we use with Qt (e.g. we generate an enum which
is also exposed and can be used in QML).

And lastly we have to register our widget so it can be initialized
when Qt parses our QML file. Qt uses QML files, json like files, in
which you define and create your GUI. Open
`qtof/QtOfExternalPlugin.cpp` and add a line with
`widgets_add_factory_for_type()` (don't forget to include your
pimpl.).

So in short these are the steps that you need to take to create
a new widgets:

- Open the `src/experimental/CodeGenerator.js` file and change the
  classname to the name of the widget you want to create. 
- Open a terminal and go into `src/experimental`
- Execute `node CodeGenerator.js`, which creates your .cpp and .h files.
- Open `src/qtof/WidgetTypes.h` and add your type
- Open `src/qtof/QtOfExternalPlugin.cpp` and add a line with 
  `widgets_add_factory_for_type()` for your type. 
- Open a terminal and go into `src/qtof/` 
- Execute `node CodeGenerator.js` 

# TODO

- Move the includes of QtOf cmake files underneath the CMAKE_PREFIX_PATH
- Remove the `ref` member from QML. 
- Figure out why the mesassge queue is called multiple times
- Check out if we can remove QtOfExternalPlugin
- Create the directory structure for DepthKit; use plugin.cmake
  - Where to store assets
  - Where to store plugin code


