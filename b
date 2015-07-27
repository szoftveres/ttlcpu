#!/bin/bash
# set -x

# make c compiler
echo "Making cc.."
cd mcc || exit 1
make clean || exit 1
make ARCH=ttlcpu all || exit 1
cd .. || exit 1

#compile
echo "Compiling program.."
cp ./mcc/output/header.asm  am/prog.asm || exit 1
cc -E ./prog.c | grep -v '^#' | ./mcc/output/mcc >> am/prog.asm || exit 1
cat ./mcc/output/footer.asm >> am/prog.asm || exit 1

# make assembler and assemble 
echo "Making AM.."
cd am || exit 1
cc -O0 -o asm am.c || exit 1

echo "Running AM.."
./asm > bytecode.h 2> asmlog.txt || exit 1
# cat asmlog.txt
cd .. || exit 1

LIMIT=32768

TOTAL=`wc -l < am/bytecode.h`
echo '*********************************************'
echo -n "Bytecode program size (bytes):  "
echo "${TOTAL}"
echo '*********************************************'

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
        cd burner/default || exit 1
        make clean || exit 1
        make all || exit 1
        cd ../.. || exit 1

        #burn
        avrdude -p m8 -c avrispmkII -P usb -B 4 -U flash:w:burner/default/burner.hex:i || exit 1

        echo
        echo "-- done with part $(($i + 1)) --"

        if (( $(( `wc -l < am/bytecode_part.h` ))  == $SCATTER )) ; then 
            echo "   more parts coming... "
            sleep 2
        fi
    fi

done

