# CRSF Serial Protocol

https://github.com/crsf-wg/crsf/wiki

## Installation

Installation is pretty straight forward. You may have to adjust the destination path in the `CRSFAnalyzer/Mac/makefile` in the `install` section.

    make
    make install

## Misc

The code is based on the KingstVIS_Analyzer_SDK that I downloaded on 19.08.2024 (KingstVIS version 3.6.3).

### macOS Apple Silicon

In order to get this compiled on macOS with an Apple Silicon processor I had to specify a different clang target (`-target x86_64-apple-darwin`) in the `makefile`. This has to be done because the `lib/libAnalyzer.dylib` was compiled on an Intel based Mac.

### IntelliSense warnings

If you are using vscode you propably getting IntelliSense warnings. This could be because your `cppStandard` is set to `c++17` by default. Setting it to `c++11` (https://github.com/trytuna/CRSF-Kingst-Analyzer/blob/ef19ad05a86a62e910794a3bbbd134fec93a006a/.vscode/c_cpp_properties.json#L15) fixes these warnings for me.
