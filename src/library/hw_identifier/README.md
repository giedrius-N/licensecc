## Linking to Other Projects

These steps are done using [CMake GUI](https://cmake.org/download/).

#### Linking Without OpenSSL

* If you do not plan to use OpenSSL in your project, ensure that OpenSSL is not linked to your `licensecc` project. If CMake detects OpenSSL, make sure to remove its directories:

![MakeSureToDeleteOpenSSLDependency](https://i.postimg.cc/k5HWHCQh/make-sure-openssl-is-not-includede.png)

* If you forget to remove these directories, issues may occur on computers without OpenSSL. It is recommended to remove them as `licensecc` functions properly without OpenSSL.

### Locating Required Files

#### `licensecc_static` Library
* After running INSTALL, not all files are copied to the install directory. For `licensecc_static` usage, go to the build directory: `...\licensecc_build\src\library\Debug\licensecc_static.lib`. The `.pdb` file will also be available. The same applies for the Release configuration.

#### `private_key.rsa`
* After configuring the project (this example uses `DEFAULT`, but it should be changed to `YOUR_PROJECT` as shown below), the private key can be found in the directory `...\licensecc\projects\DEFAULT`. This file is required for generating licenses.

#### Includes
* Includes are properly copied to the install directory after running INSTALL: `...\licensecc_install\include`.

**Note**: You may encounter issues in `datatypes.h` complaining about a missing `licensecc_properties.h`. To resolve this, edit `datatypes.h` and change:
```cpp
#include <licensecc_properties.h>
```
To:
```cpp
#include "YOUR_PROJECT/licensecc_properties.h"
```
This ensures the file is located correctly.

## Example of License Generation

Here is a quick guide on creating a license.

#### LCCInspector

* Navigate to the install directory, where you will find `inspector.exe`: `...\licensecc_install\bin\DEFAULT\lccinspector.exe`. Run it in the console, and you will see output similar to the following:
```
DEFAULT:AAAw-ZezJ-DIA=
MAC:AAAw-ZezJ-DIA=
IP:ACDA-qDwv-Kio=
Disk:AEtT-ljYA-AAA=
Motherboard_disk:ALjR-zF78-1cg=
```
**Note**: Replace `DEFAULT` with `YOUR_PROJECT` for proper usage:

![YOUR_PROJECT](https://i.postimg.cc/g04GccSN/your-project.png)

#### Generating a License

* Use the retrieved ID, for example: `ALjR-zF78-1cg=`.
* Navigate to `...\licensecc_install\bin\lccgen.exe`.
* Issue the license with the following command:
```bash
.\lccgen.exe license issue -e 20250125 -o license.lic --client-signature ALjR-zF78-1cg= --primary-key .\private_key.rsa -f YOUR_PROJECT
```
* This will generate the `.lic` file required for identification.
* For more information, run:
```bash
.\lccgen.exe license issue --help
```

## Later Usage in the Project

* When writing your license validator, ensure the project name matches the name used during license file generation:
```cpp
// The project name must match the one specified during license file generation.
CallerInformations callerInfo = { "\0", "YOUR_PROJECT", 0 };
```

* If you want the user to send their unique computer identifier, apply the same strategy here:
```cpp
identify_pc(STRATEGY_MOTHERBOARD_DISK, pcIdentifier, &pcIdSize, nullptr);
```

### Motherboard Disk Strategy

* If this strategy is used, [HWInfo](https://github.com/lfreist/hwinfo) must be linked to your project.
* Required components include the motherboard and disk:
```cmake
debug;${hwinfo_path}//lib/Debug/hwinfo_disk;optimized;${hwinfo_path}//lib/Release/hwinfo_disk
debug;${hwinfo_path}//lib/Debug/hwinfo_mainboard;optimized;${hwinfo_path}//lib/Release/hwinfo_mainboard
```
* If this fails, try including all library components (RAM, CPU, etc.).

## Troubleshooting

### License Corrupted
* If you encounter a "license is corrupted" message, ensure you use the same private key, public key, and compiled library files for `YOUR_PROJECT`.
* Any mismatch will result in a corrupted license. It is recommended to clean and rebuild the project, excluding OpenSSL, and ensure `YOUR_PROJECT` is set as the project name.