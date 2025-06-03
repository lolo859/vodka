#!/bin/bash
echo "Automated Linker and Compilation Of Highly Object Oriented Language"
if command -v g++ >/dev/null 2>&1; then
    COMPILER="g++"
    VERSION=$($COMPILER -dumpversion)
else
    echo "g++ isn't detected and clang isn't supported for the moment. Exiting."
    exit 1
fi
echo "Using $COMPILER ($VERSION)"
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
        g++ -c vodka-lib/vodka-errors.cpp -o vodka-errors.o
        echo "Compiled vodka-errors"
        g++ -c vodka-lib/vodka-variables.cpp -o vodka-variables.o
        echo "Compiled vodka-variables"
        g++ -c vodka-lib/vodka-internal-library/vil-kernel.cpp -o vil-kernel.o
        echo "Compiled vil-kernel"
        g++ -c vodka-lib/vodka-internal-library/vil-conversions.cpp -o vil-conversions.o
        echo "Compiled vil-conversions"
        g++ -c vodka-lib/vodka-internal-library/vil-math.cpp -o vil-math.o
        echo "Compiled vil-math"
        g++ -c vodka-lib/vodka-internal-library/vil-vodstr.cpp -o vil-vodstr.o
        echo "Compiled vil-vodstr"
        ar rcs libvodka.a vodka-json.o vodka-type.o vodka-syscalls.o vodka-utilities.o vodka-analyser.o vodka-errors.o vodka-variables.o vil-kernel.o vil-conversions.o vil-math.o vil-vodstr.o
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
        g++ -c vodka-lib/vodka-errors.cpp -o vodka-errors.o
        echo "Compiled vodka-errors"
        g++ -c vodka-lib/vodka-variables.cpp -o vodka-variables.o
        echo "Compiled vodka-variables"
        g++ -c vodka-lib/vodka-internal-library/vil-kernel.cpp -o vil-kernel.o
        echo "Compiled vil-kernel"
        g++ -c vodka-lib/vodka-internal-library/vil-conversions.cpp -o vil-conversions.o
        echo "Compiled vil-conversions"
        g++ -c vodka-lib/vodka-internal-library/vil-math.cpp -o vil-math.o
        echo "Compiled vil-math"
        g++ -c vodka-lib/vodka-internal-library/vil-vodstr.cpp -o vil-vodstr.o
        echo "Compiled vil-vodstr"
        ar rcs libvodka.a vodka-json.o vodka-type.o vodka-syscalls.o vodka-utilities.o vodka-analyser.o vodka-errors.o vodka-variables.o vil-kernel.o vil-conversions.o vil-math.o vil-vodstr.o
        echo "Library built under libvodka.a"
        g++ vodka.cpp -o vodka -L. -lvodka
        echo "Vodka built under vodka"
        rm vodka-utilities.o vodka-type.o vodka-syscalls.o vodka-json.o vodka-analyser.o vil-kernel.o vodka-errors.o vodka-variables.o vil-conversions.o libvodka.a vil-math.o vil-vodstr.o
        ;;
    vodka)
        echo "Building Vodka"
        g++ vodka.cpp -o vodka -L. -lvodka
        echo "Vodka built under vodka"
        ;;
    clean)
        echo "Cleaning up"
        rm vodka-utilities.o vodka-type.o vodka-syscalls.o vodka-json.o vodka-analyser.o vil-kernel.o vodka-errors.o libvodka.a vil-conversions.o vodka-variables.o vil-math.o vil-vodstr.o
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
        g++ -c vodka-lib/vodka-errors.cpp -o vodka-errors.o
        echo "Compiled vodka-errors"
        g++ -c vodka-lib/vodka-variables.cpp -o vodka-variables.o
        echo "Compiled vodka-variables"
        g++ -c vodka-lib/vodka-internal-library/vil-kernel.cpp -o vil-kernel.o
        echo "Compiled vil-kernel"
        g++ -c vodka-lib/vodka-internal-library/vil-conversions.cpp -o vil-conversions.o
        echo "Compiled vil-conversions"
        g++ -c vodka-lib/vodka-internal-library/vil-math.cpp -o vil-math.o
        echo "Compiled vil-math"
        g++ -c vodka-lib/vodka-internal-library/vil-vodstr.cpp -o vil-vodstr.o
        echo "Compiled vil-vodstr"
        ar rcs libvodka.a vodka-json.o vodka-type.o vodka-syscalls.o vodka-utilities.o vodka-analyser.o vodka-errors.o vodka-variables.o vil-kernel.o vil-conversions.o vil-math.o vil-vodstr.o
        echo "Library built under libvodka.a"
        echo "Building Vodka"
        g++ vodka.cpp -o vodka -L. -lvodka
        echo "Vodka built under vodka"
        rm vodka-utilities.o vodka-type.o vodka-syscalls.o vodka-json.o vodka-analyser.o vil-kernel.o vodka-errors.o vodka-variables.o libvodka.a vil-conversions.o vil-math.o vil-vodstr.o
        ;;
esac