./build.sh
if [ $# -eq 0 ]; then
    ./main
    exit 1
fi


./main > $1.ppm
