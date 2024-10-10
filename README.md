# vodka
Vodka Objective Dictionary for Kernel Analyser

This is the new vodka translator into kernel code for Shelter (not realeased yet). It's based on the first concept of vodka (see lolo859/vodka-old) but build in C++ this time. The new vodka translator will only translate high level vodka code to low level kernel code. The developpement for th interpreter for kernel code will start soon. It's based on the same syntax with a few exeception. There are a few different symbol that each start with £ :

- £VODSTART
- £VODEND
- £VODTYPE

For the moment you can only create cell, print variable and vodint variable :

```
£VODTYPE shell
£VODSTART
vodka a=vodint 5
vodprint a
£VODEND
```

The full documentation will be realeased in a few weeks.
