# Build first (if needed): make

# Basics
../bin/grepx "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx "Data Science2" ./folder_stage1/datascience.txt ./folder_stage1/datascience2.txt
../bin/grepx "Data Science2" ./folder_stage1/*.txt
../bin/grepx -r "Data Science" ./folder_stage1

# Output and filters
../bin/grepx -n "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx -c "Data Science2" ./folder_stage1/*.txt
../bin/grepx -l "Data Science2" ./folder_stage1/*.txt
../bin/grepx -v "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx -i "data science2" ./folder_stage1/datascience.txt
../bin/grepx -q "Data Science2" ./folder_stage1/datascience.txt; echo $?

# Multiple patterns and regex
../bin/grepx -e "Data Science2" -e "Cyber Security" ./folder_stage1/datascience.txt
../bin/grepx "Science[0-9]" ./folder_stage1/*.txt
../bin/grepx "hahaha.*hihihi" ./folder_stage1/datascience.txt

# STDIN
echo "Data Science2 und mehr" | ../bin/grepx "Science2"
cat ./folder_stage1/datascience.txt | ../bin/grepx "Data Science"

# Edge cases / validation
../bin/grepx -e "" ./folder_stage1/datascience.txt
../bin/grepx -q "NOT_THERE" ./folder_stage1/datascience.txt; echo $?
../bin/grepx -r
../bin/grepx -C -1 "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx -q -c "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx -q -l "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx -q -n "Data Science2" ./folder_stage1/datascience.txt
../bin/grepx -c -n "Data Science2" ./folder_stage1/datascience.txt

