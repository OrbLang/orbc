# Orbc - A compiler for Orb

## !! WARNING !!
This project isn't done yet, and is currently in a non-functioning state.
We are actively working on it though.

## Usage

To compile a simple project, supply the `orbc` compiler with all the files that will be used by compilation,
where the entry file is the first one.

```bash
$ orbc ponder.orb foo.orb bar.orb
```
This will output an executable called `ponder` into your current working directory.
You can also specify the name with the `-o` flag.

```bash
$ orbc ponder.orb foo.orb bar.orb -o baz
```
Here the outputted executable will be called `baz`

## Installation
To install, you will need to clone the repository and compile it yourself.
To compile orbc, we use `meson`, which you will need to have installed on your system.

#### 1. Clone the repo
```bash
$ git clone https://github.com/OrbLang/orbc
$ cd orbc
```

#### 2. Then set your c++ compiler of choice, like `clang++`:
```bash
$ export CXX=clang++; export CC=clang
```

#### 3. After that, use `meson` to setup a build directory, by running the following command:
You can change the prefix to where you want orbc to be installed.
If left blank it will default to `/usr/local` and `C:\` on Windows.
You can also change the buildtype if desired, for example to `debug` or `debugoptimized`
```bash
$ meson setup builddir --prefix=$PWD/install --buildtype release
```
_(If you wish for `orbc` to not output any color, the flag `-Dno-color=true` can be used)_

#### 4. After that, you only need to compile the project.
```bash
$ meson compile -C builddir
```
Now the `orbc` executable should be in `./builddir/orbc` 
#### 5. And if you wish, you can install orbc and dependencies with the install command
```bash
$ meson install -C builddir
```
And orbc will end up in a `bin` folder at the prefix specified in step 3. _(If no prefix orbc will be at `/usr/local/bin`)_
