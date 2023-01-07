# potkuri
Simple kick drum plugin...for fun

## Installation and compiling
JUCE is used as submodule in this project. To get it working locally clone this project normally. Then in project folder run:

```git submodule init```

and

```git submodule update```

These command fetch JUCE from git reposity for compiling.
## Compiling
Compile using Cmake configuration found from `CMakeList.txt`
## Running
Plugin artefacts can be found from:
`/build/AudioPluginExample_artefacts/JuceLibraryCode/<artefact_type_specific_folder>`