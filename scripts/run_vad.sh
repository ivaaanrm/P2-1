#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh

# Write here the name and path of your program and database
HOMELOCAL=$HOME/Desktop/Uni/3B          # Path to your home directory !!!
DIR_P2=$HOMELOCAL/PAV/P2-1
DB=$DIR_P2/db.v4
CMD="$DIR_P2/bin/vad --alpha1=${1:-8} --alpha2=${2:-8}"

for filewav in $DB/*/*wav; do
#    echo
    echo "**************** $filewav ****************"
    if [[ ! -f $filewav ]]; then 
	    echo "Wav file not found: $filewav" >&2
	    exit 1
    fi

    filevad=${filewav/.wav/.vad}

    $CMD -i $filewav -o $filevad || exit 1

# Alternatively, uncomment to create output wave files
#    filewavOut=${filewav/.wav/.vad.wav}
#    $CMD $filewav $filevad $filewavOut || exit 1

done

perl scripts/vad_evaluation.pl $DB/*/*lab

exit 0
