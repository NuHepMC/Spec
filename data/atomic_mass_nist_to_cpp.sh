#!/bin/bash

# Aim is to convert NIST table to a c++11 map initializer
# std::map<char,std::map<char,double>> MassTable = {{<Z>,{{<A>, <AM>},}},};

IFSO=${IFS}
Z=0
NAME=
A=
AM=
echo "#include <map>"
echo "std::map<char,std::map<char,double>> const MassTable{"
while IFS= read -r line
do
  CSL=$(echo $line | sed "s/\s\+/ /g")
  if [ ${CSL:0:1} == "#"  ]; then
    continue
  fi

  if [ ${CSL:0:1} == "_"  ]; then
    if [ $Z != 0 ]; then # end previous element
      echo -e "\t\t}\n\t},"
    fi
    Z=0
    continue
  fi

  if [ $Z == 0 ]; then # new element
    set -- ${CSL}
    if ! [[ "$1" =~ [0-9]+ ]]; then
      continue
    fi
    NAME=$2
    Z=$1
    A=$3
    AM=$(echo $4 | sed "s/(.*//g")
    # echo "New element Name $NAME: Z = $Z, A = $A AM = $AM"
    echo -e "\t//${NAME}"
    echo -e "\t{$Z,\n\t\tstd::map<char,double>{"
    echo -e "\t\t\t{$A, $AM},"
  else
    set -- ${CSL}
    if [[ "$1" =~ [0-9]+ ]]; then
      # echo "No Name: $1"
      A=$1
      AM=$(echo $2 | sed "s/(.*//g")
    else # Have a named isotope
      # echo "Name ($1), try $3"
      A=$2
      AM=$(echo $3 | sed "s/(.*//g")
    fi
    # echo -e "\tNew isotope Name Z = $Z, A = $A AM = $AM"
    echo -e "\t\t\t{$A, $AM},"
  fi
done < atomic_mass_nist.txt
echo "};"
