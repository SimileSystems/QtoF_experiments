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
- Select _CMake_
- Click _Change..._ for the _CMake Generator_ (at the bottom).
    - **Mac:**
      - Generator: _Unix Makefiles_
      - Extra generator: _CodeBlocks_
    - **Win: (defaults)**
      - Generator: _NMake MakeFiles_
      - Extra generator: _CodeBlocks_

## Opening a CMake project in QtCreator

- Open Qt Creator
- _File_ > _Open File or Project_
- Select your CMakeLists.txt file (e.g. `apps/empty/build/CMakeLists.txt`)
- Select _[x] CMake_, then press the _Configure Project_ button.
- Open the `CMakeLists.txt` file and change the _project()_ value.
- _Build_ > _Build All_ 
