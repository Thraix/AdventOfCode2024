#/bin/sh

if [ ! -e cookies.txt ]
then
  echo "No cookies.txt file exists"
  exit 1
fi

list=$(find -name Day*.cpp | awk -F '/' '{print $5}' | awk -F '.' '{print $1}' | sed 's/Day//g')

for item in $list
do
  dir="AdventOfCode2024/res/day$item/input.txt"
  if [ ! -e $dir ]
  then
    webitem=$(echo $item | sed 's/^0//g')
    echo "Fetching $dir"
    mkdir -p "AdventOfCode2024/res/day$item/"
    touch "AdventOfCode2024/res/day$item/test_input.txt"
    curl -s --cookie cookies.txt https://adventofcode.com/2024/day/$webitem/input > $dir
  fi
done

