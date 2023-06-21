#!/bin/bash

co=0

rec() {
    for i in "$1"/*; do
        if [ -d "$i" ]; then
            rec "$i"
        elif [ -f "$i" ]; then
            echo "$i"
            var=$(ls -l --time=atime "$i")
            echo "$var"
            
            perm1="${var:3:1}"
            
            perm2="${var:6:1}"
            
            perm3="${var:9:1}"
            
            basefile=$(basename "$i")
            

            if [ "$perm1" == "x" ]; then
                a="out_dir/$USER/"
                if [ ! -d "$a" ]; then
                    mkdir -p "$a"
                fi
                newfile="$basefile"
                cp "$i" "$a/$newfile"
                chmod -x "$a/$newfile"
               
            fi
            
            if [[ $perm2 == "x" || $perm3 == "x" ]]; then
                b="out_dir/others/"
                if [ ! -d "$b" ]; then
                    mkdir -p "$b"
                fi
                newfile="$basefile"
                cp "$i" "$b/$newfile"
                chmod -x "$b/$newfile"
                
            fi
        fi
    done
}

mkdir -p "out_dir/$USER"
rec "in_dir"


filefolder=$(ls -t "out_dir/$USER")

n=0
for files in $filefolder; do
    ((n++))
    
    mv "out_dir/$USER/$files" "out_dir/$USER/${n}_$(basename "$files")"
   
done


filefolder=$(ls -t "out_dir/others")

n=0
for files in $filefolder; do
    ((n++))
    
    mv "out_dir/others/$files" "out_dir/others/${n}_$(basename "$files")"
   
done

