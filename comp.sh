#!/bin/bash
echo "Automated Linker and Compilation Of Highly Object Oriented Language"
case "$1" in
    library)
        echo "Building Library"
        g++ -c vodka-lib/vodka-utilities.cpp -o vodka-utilities.o
        echo "Compiled vodka-utilities"
        g++ -c vodka-lib/vodka-type.cpp -o vodka-type.o
        echo "Compiled vodka-type"
        g++ -c vodka-lib/vodka-syscalls.cpp -o vodka-syscalls.o
        echo "Compiled vodka-syscalls"
        g++ -c vodka-lib/vodka-json.cpp -o vodka-json.o
        echo "Compiled vodka-json"
        g++ -c vodka-lib/vodka-analyser.cpp -o vodka-analyser.o
        echo "Compiled vodka-analyser"
        g++ -c vodka-lib/vodka-instructions.cpp -o vodka-instructions.o
        echo "Compiled vodka-instructions"
        g++ -c vodka-lib/vodka-errors.cpp -o vodka-errors.o
        echo "Compiled vodka-errors"
        g++ -c vodka-lib/vodka-internal-library/vil-kernel.cpp -o vil-kernel.o
        echo "Compiled vil-kernel"
        ar rcs libvodka.a vodka-json.o vodka-type.o vodka-syscalls.o vodka-utilities.o vodka-analyser.o vodka-instructions.o vodka-errors.o vil-kernel.o
        echo "Library built under libvodka.a"
        ;;
    all)
        echo "Building vodka transcoder"
        echo "Building Library"
        g++ -c vodka-lib/vodka-utilities.cpp -o vodka-utilities.o
        echo "Compiled vodka-utilities"
        g++ -c vodka-lib/vodka-type.cpp -o vodka-type.o
        echo "Compiled vodka-type"
        g++ -c vodka-lib/vodka-syscalls.cpp -o vodka-syscalls.o
        echo "Compiled vodka-syscalls"
        g++ -c vodka-lib/vodka-json.cpp -o vodka-json.o
        echo "Compiled vodka-json"
        g++ -c vodka-lib/vodka-analyser.cpp -o vodka-analyser.o
        echo "Compiled vodka-analyser"
        g++ -c vodka-lib/vodka-instructions.cpp -o vodka-instructions.o
        echo "Compiled vodka-instructions"
        g++ -c vodka-lib/vodka-errors.cpp -o vodka-errors.o
        echo "Compiled vodka-errors"
        g++ -c vodka-lib/vodka-internal-library/vil-kernel.cpp -o vil-kernel.o
        echo "Compiled vil-kernel"
        ar rcs libvodka.a vodka-json.o vodka-type.o vodka-syscalls.o vodka-utilities.o vodka-analyser.o vodka-instructions.o vodka-errors.o vil-kernel.o
        echo "Library built under libvodka.a"
        g++ vodka.cpp -o vodka -L. -lvodka
        echo "Vodka built under vodka"
        rm vodka-utilities.o vodka-type.o vodka-syscalls.o vodka-json.o vodka-analyser.o vil-kernel.o vodka-instructions.o vodka-errors.o libvodka.a
        ;;
    vodka)
        echo "Building Vodka"
        g++ vodka.cpp -o vodka -L. -lvodka
        echo "Vodka built under vodka"
        ;;
    clean)
        echo "Cleaning up"
        rm vodka-utilities.o vodka-type.o vodka-syscalls.o vodka-json.o vodka-analyser.o vil-kernel.o vodka-instructions.o vodka-errors.o libvodka.a
        ;;
    *)
        echo "Building vodka transcoder"
        echo "Building Library"
        g++ -c vodka-lib/vodka-utilities.cpp -o vodka-utilities.o
        echo "Compiled vodka-utilities"
        g++ -c vodka-lib/vodka-type.cpp -o vodka-type.o
        echo "Compiled vodka-type"
        g++ -c vodka-lib/vodka-syscalls.cpp -o vodka-syscalls.o
        echo "Compiled vodka-syscalls"
        g++ -c vodka-lib/vodka-json.cpp -o vodka-json.o
        echo "Compiled vodka-json"
        g++ -c vodka-lib/vodka-analyser.cpp -o vodka-analyser.o
        echo "Compiled vodka-analyser"
        g++ -c vodka-lib/vodka-instructions.cpp -o vodka-instructions.o
        echo "Compiled vodka-instructions"
        g++ -c vodka-lib/vodka-errors.cpp -o vodka-errors.o
        echo "Compiled vodka-errors"
        g++ -c vodka-lib/vodka-internal-library/vil-kernel.cpp -o vil-kernel.o
        echo "Compiled vil-kernel"
        ar rcs libvodka.a vodka-json.o vodka-type.o vodka-syscalls.o vodka-utilities.o vodka-analyser.o vodka-instructions.o vodka-errors.o vil-kernel.o
        echo "Library built under libvodka.a"
        echo "Building Vodka"
        g++ vodka.cpp -o vodka -L. -lvodka
        echo "Vodka built under vodka"
        rm vodka-utilities.o vodka-type.o vodka-syscalls.o vodka-json.o vodka-analyser.o vil-kernel.o vodka-instructions.o vodka-errors.o libvodka.a
        ;;
esac