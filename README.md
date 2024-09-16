# Orbc - A compiler for Orb

## !! WARNING !!
This project isn't done yet, and is currently in a non-functioning state.
We are actively working on it though.

## Usage

To compile a simple project, supply the `orbc` compiler with all the files that will be used by compilation,
where the entry file is the first one.

```bash
$ orbc main.orb foo.orb bar.orb
```
This will output an executable called `main` into your current working directory.
You can also specify a specific output path with the `-o` flag.

```bash
$ orbc main.orb foo.orb bar.orb -o baz
```
Here the outputted executable will be called `baz`

## Installation
To install, either download the binaries of the latest release, or you can manually install it.

### Manual installation

To manually compile the compiler, you need to make sure you have `meson` installed.

Clone the repo
```bash
$ git clone https://github.com/OrbLang/orbc
$ cd orbc
```

Then set your c++ compiler of choice, like `clang++`:
```bash
$ CXX='clang++'
```

After that, use `meson` to setup a builddir, by running:
```bash
$ meson setup builddir
```
(If you wish for `orbc` to not output any color, use the `-Dno-color=true` flag here)

After that, you only need to compile the project.
```bash
$ meson compile -C builddir
```
Now the `orbc` executable should be in `./builddir/orbc` 
