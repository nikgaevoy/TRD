cd $(dirname $0)

cp ../* .

files=`ls *.cpp`

echo $files

for file in $files ; do
	clang-format -i -style="{BasedOnStyle: Google, IndentWidth: 2, ColumnLimit: 59}" $file ;
done
