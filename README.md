# Orbc - A compiler for Orb

## ⚠️ !! WARNING !! ⚠️
This project isn't done yet, and is currently in a non-functioning state.
We are actively working on it though.

## Usage
To compile a simple project, supply the `orbc` compiler with all your files containing orb source code.
```bash
$ orbc ponder.orb foo.orb bar.orb
```
This will output an executable called `ponder`, copying the first source file filename.
To specify the name of the output, supply the `-o` flag, followed by the name of the output.

```bash
$ orbc ponder.orb foo.orb bar.orb -o baz
```
Here the output will be named `baz`

## Installation
To install orbc, you will need to clone the repository and compile it yourself.
We use the `meson` buildsystem, which you will need to have installed on your system.

#### 1. Clone the repo:
```bash
$ git clone https://github.com/OrbLang/orbc
$ cd orbc
```

#### 2. Specify what  c++ compiler and linker to use:
Here llvm's clang and lld are used
```bash
$  export CC=clang; export CXX=clang++; export CC_LD=lld; export CXX_LD=lld
```
_this can also be done, without exporting, in the next step: `CC=clang CXX=clang++ meson setup ...`_

#### 3. Setup a build directory:
You can change the prefix to where you want orbc to be installed.
If left out it will default to `/usr/local` and `C:\` on Windows.
You can also change the buildtype if desired, for example to `--buildtype debug` or `--buildtype debugoptimized`
```bash
$ meson setup builddir --prefix=$PWD/install
```
_(If you wish for `orbc` to not output any color, the flag `-Dno-color=true` can be used)_

#### 4. Compile orbc:
```bash
$ meson compile -C builddir
```
Now the `orbc` executable should be in `./builddir/orbc` 
#### 5. Optionally install orbc and dependencies on your system:
```bash
$ sudo meson install -C builddir
```
Now orbc will end up in a `bin` folder at the prefix specified in step 3. _(If no prefix orbc will be at `/usr/local/bin`)_
