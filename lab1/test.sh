#!/bin/bash

echo "Welcome my son, welcome to the machine!"
echo "Sit back and enjoy while I run some tests on your code."

make clean &> /dev/null
make &> /dev/null

testovi=$(ls testovi)
#testovi="moj"

tocno=0
krivo=0
time_sum=0
lista_krivih=""

for test in $testovi
do
  INPUT="../testovi/"$test"/test.in"
  OUTPUT="testovi/"$test"/test.out"
  LAN="testovi/"$test"/test.lan"
  
  ./generator < $LAN &> /dev/null
  cd analizator
  
  make clean &> /dev/null
  make &> /dev/null

  (time ./analizator > moj_izlaz.txt < $INPUT 2> /dev/null) &> temp_time.txt
  seconds=$(cat temp_time.txt| grep real |cut -f2 -d'm' |cut -f1 -d's')
  time_sum=$(echo "$time_sum + $seconds" | bc)

  cd ..

  printf "Result for the test case %-20s> TIME: %s s RESULT: " $test $seconds
  diff analizator/moj_izlaz.txt $OUTPUT > /dev/null

  if [ $? -eq 1 ] 
  then
    echo "WA"
    krivo=$((krivo+1))
    lista_krivih=$lista_krivih" "$test
  else
    echo "OK"
    tocno=$((tocno+1))
  fi
done

echo "STATS: $tocno / $((tocno+krivo))"
echo "TIME: "$time_sum
if [ $krivo -gt 0 ]
then
  echo "FAILED: "$lista_krivih
fi

echo "Cleaning up .."

rm analizator/temp_time.txt
rm analizator/moj_izlaz.txt
cd analizator
make clean &> /dev/null

cd ..
make clean &> /dev/null

echo "Done"
