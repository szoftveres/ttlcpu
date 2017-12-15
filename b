#!/bin/bash
# set -x

ARCHITECTURE=ttlcpu

error () {
    echo "ERROR:"
    tail -n 5 build.log
    exit 1
}

# make c compiler
echo "Making CC for ${ARCHITECTURE} .."
cd mcc || exit 1
make clean > ../build.log || exit 1
make "ARCH=${ARCHITECTURE}" all >> ../build.log || error
cd .. || exit 1

if test -z "${1}" ; then
    echo No file specified, exiting
    exit 0
fi

#compile
echo "Running CC, '${1}' .."
cp "./mcc/arch/${ARCHITECTURE}/header.asm"  "am/program_${ARCHITECTURE}.s" || exit 1
cc -E "${1}" | grep -v '^#' | ./mcc/mcc >> "am/program_${ARCHITECTURE}.s" 2>> build.log || error
cat "./mcc/arch/${ARCHITECTURE}/footer.asm" >> "am/program_${ARCHITECTURE}.s" || exit 1

# make assembler and assemble 
echo "Making AM .."
cd am || exit 1
cc -O0 -o asm am.c >> ../build.log || error

echo "Running AM .."
./asm > bytecode.h 2>> ../build.log || error
cd .. || exit 1

LIMIT=32768

TOTAL=`wc -l < am/bytecode.h`
echo -n "Binary size: "
echo -n "${TOTAL} "
echo "bytes"
echo '*************************'

if (( $TOTAL > $LIMIT )) ; then
    echo
    echo "ERROR: Program exceeds program space limit (${LIMIT} bytes)!"
    exit 1
fi

SCATTER=7680      # 7.5k

#burn
echo Press ENTER to burn
read EMPTYLINE

for (( i = 0; i < (($TOTAL / $SCATTER) + 1)  ; i++ )) ; do

    cat am/bytecode.h | tail -n $(( $TOTAL - ($i * $SCATTER) ))  | head -n $(( $SCATTER )) > am/bytecode_part.h

    if (( $(( `wc -l < am/bytecode_part.h` ))  != 0 )) ; then 
        echo
        echo "-- part $(($i + 1)) --"

        # make burner
        cd burner || exit 1
        make clean || exit 1
        make all || exit 1
        cd .. || exit 1

        #burn
        avrdude -p m8 -c avrispmkII -P usb -B 4 -U flash:w:burner/burner.hex:i || exit 1

        echo
        echo "-- done with part $(($i + 1)) --"

        if (( $(( `wc -l < am/bytecode_part.h` ))  == $SCATTER )) ; then 
            echo "   more parts coming... "
            sleep 2
        fi
    fi

done

