# QtoF Experiments

This repository was created as a research project to integrate
openFrameworks into an Qt application. During the development of this
project we had to make a couple of big structural changes. Most of the
header files are documented and give insight on the inner workings of
the code.

One major issue we encountered was integrating custom plugins into Qt
Quick Designer. Long story short: custom plugins are not supported by
Qt at the moment but we found a way to get it working. We are creating
a _fake_ plugin which only purpose is integrating our widgets into Qt
Quick Designer.

This repository is to be used within another
project. [SEE THIS EXAMPLE PROJECT]. Most of the setup/installation is
described in this repository.
