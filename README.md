# vodka
Vodka Objective Dictionary for Kernel Analyser

This is the new vodka translator into kernel code for Shelter (not realeased yet). It's based on the first concept of vodka (see lolo859/vodka-old) but build in C++ this time. The new vodka translator will only translate high level vodka code to low level kernel code. The developpement for the interpreter for kernel code will start soon. It's based on the same syntax with a few exeception. To compile vodka to kernel code :

```
vodka -s test.vod -o test.k
```

To see the full list of argument :

```
vodka -h
```

To compile, use the ```comp.sh``` file. To compile only library, use ```comp.sh library```

## Install

The only package manager to have Vodka is `dnf`, you can install Vodka like this (for now, there is only the beta channel):

```
sudo dnf copr enable lolo859/vodka-beta
sudo dnf install vodka-beta
```

The executable will be named `vodka-beta`. To install the library (only .a and header file):

```
sudo dnf copr enable lolo859/vodka-lib-beta
sudo dnf install vodka-lib-beta
```


Copr Proof
