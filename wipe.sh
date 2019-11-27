touch {a..l}.cpp

for file in ?.cpp ; do
    cat template.cpp > $file ;
done
