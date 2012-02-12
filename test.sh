#gcc 

#1. should have /dev/cdata
#2. should install ./cdata.ko
rm test
gcc -o test test.c

./uptime.sh& 
time sudo ./test
exit 0
