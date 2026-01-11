cd ..
if [ ! -f test_nn.c ]; then
    echo "File test_nn.c not found. Make sure you call this script from scripts folder."
    exit 1
fi

NAME=test_nn
FOLDER=temp_outputs/
START=0
END=9
CNT=$((END - START + 1))

mkdir -p ${FOLDER}

make clean > make_log.log
if make -j 4 > make_log.log 2>&1
then
    echo "Starting"
else
    echo "Build failed. Check make_log.log under ${FOLDER}"
    exit 1
fi

# Generate outputs and save in ${FOLDER}
for i in $(seq ${START} ${END})
do
    echo "Doing input ${i}"
    if [[ "$OSTYPE" == "darwin"* ]]; then
        sed -i '' "{s/#include \"emnist_byclass_testset_0_999\/input_[[:digit:]]*\.h\"/#include \"emnist_byclass_testset_0_999\/input_${i}.h\"/;}" test_nn.c
        sed -i '' "{s/#define INPUT_NUM [[:digit:]]*/#define INPUT_NUM ${i}/;}" test_nn.c
    else
        sed -i "{s/#include \"emnist_byclass_testset_0_999\/input_[[:digit:]]*\.h\"/#include \"emnist_byclass_testset_0_999\/input_${i}.h\"/;}" test_nn.c
        sed -i "{s/#define INPUT_NUM [[:digit:]]*/#define INPUT_NUM ${i}/;}" test_nn.c
    fi
    rm ${NAME}
    rm obj/${NAME}.o
    make -j 4 > make_log.log 2>&1
    ./${NAME} > ${FOLDER}/out_${i} 2>&1
done

# Check each file and compute accuracy
REGEX="^Label: ([0-9]+), Max: ([0-9]+).*"
HIT=0

for i in $(seq ${START} ${END})
do
    while IFS= read -r LINE; do
        if [[ "${LINE}" =~ ${REGEX} ]]
        then
            LABEL=${BASH_REMATCH[1]}
            MAX=${BASH_REMATCH[2]}
            if [[ "$LABEL" == "$MAX" ]]
            then
                HIT=$((HIT + 1))
            fi
        fi
    done < "${FOLDER}/out_${i}"
done
echo "Correct: ${HIT} out of ${CNT}"

rm make_log.log
rm -r ${FOLDER}