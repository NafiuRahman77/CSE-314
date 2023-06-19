#!/bin/bash

# Check if the required arguments are provided
if [ "$#" -lt 4 ]; then
    echo "Usage:"
    echo "$0 <submissions_folder> <targets_folder> <tests_folder> <answers_folder> [-v] [-noexecute]"
    echo "-v: verbose"
    echo "-noexecute: do not execute code files"
    exit 1 
fi

# Retrieve the arguments
source_folder="$1"
target_folder="$2"
test_folder="$3"
answer_folder="$4"

if [ -d "$target_folder" ]; then
    rm -rf "$target_folder"
fi


mkdir -p "$target_folder"

noexecute=false
verbose=false

# Check for optional arguments
while [[ "$#" -gt 4 ]]; do
    option="$5"
    shift

    if [ "$option" = "-v" ]; then
        verbose=true
    elif [ "$option" = "-noexecute" ]; then
        noexecute=true
    else
        #echo "Unknown option: $option"
        exit 1
    fi
done


# Specify the target folders
target_c_folder="$target_folder/C"
target_java_folder="$target_folder/Java"
target_python_folder="$target_folder/Python"
mkdir -p "$target_c_folder"
mkdir -p "$target_java_folder"
mkdir -p "$target_python_folder"



if ! $noexecute; then
result_file="$target_folder/result.csv"
echo "student_id,type,matched,not_matched" > "$result_file"
fi

count=0

for test_file in "$test_folder"/*; do
    if [ -f "$test_file" ]; then
      ((count++))             
    fi
done

if $verbose; then
    echo "Found $count test files "
fi

for subdirectory in "$source_folder"/*; do

    base=$(basename "$subdirectory")
    roll="${base:(-11):7}"
    #echo "Roll: $roll"
    if $verbose; then
        echo "Organizing files of $roll"
    fi

    temp_dir=$(mktemp -d)
    unzip -q "$subdirectory" -d "$temp_dir"

    code_file=$(find "$temp_dir" -type f \( -name "*.c" -o -name "*.java" -o -name "*.py" \))

    if [[ $code_file == *.c ]]; then
        language="C"
        extension="c"
    elif [[ $code_file == *.java ]]; then
        language="Java"
        extension="java"
    elif [[ $code_file == *.py ]]; then
        language="Python"
        extension="py"
    else
        language="unknown"
        extension=""
    fi

    if [[ $language == "C" ]]; then
        new_file="main.c"
        roll_folder="$target_c_folder/$roll"
        mkdir -p "$target_c_folder/$roll"
        mv "$code_file" "$target_c_folder/$roll/$new_file"
    elif [[ $language == "Java" ]]; then
        new_file="main.java"
        roll_folder="$target_java_folder/$roll"
        mkdir -p "$target_java_folder/$roll"
        mv "$code_file" "$target_java_folder/$roll/$new_file"
    elif [[ $language == "Python" ]]; then
        new_file="main.py"
        roll_folder="$target_python_folder/$roll"
        mkdir -p "$target_python_folder/$roll"
        mv "$code_file" "$target_python_folder/$roll/$new_file"
    else
        #echo "Unsupported language: $language"
        exit 1
    fi


    if ! $noexecute; then
        if [ "$language" = "C" ]; then
            gcc "$target_c_folder/$roll/$new_file" -o "$target_c_folder/$roll/main.out"
        elif [ "$language" = "Java" ]; then
            javac "$target_java_folder/$roll/$new_file" -d "$target_java_folder/$roll/"
        fi

        matched_tests=0
        failed_tests=0

        if $verbose; then
            echo "Executing files of $roll"
        fi

        for test_file in "$test_folder"/*; do
            if [ -f "$test_file" ]; then

                test_filename=$(basename "$test_file")
                test_number="${test_filename:(-5):1}"
                output_file="$roll_folder/out$test_number.txt"
                touch "$output_file"
                
                if [ "$extension" = "c" ]; then
                    "$target_c_folder/$roll/main.out" < "$test_file" > "$output_file"
                elif [ "$extension" = "java" ]; then
                    java -cp "$target_java_folder/$roll/" Main < "$test_file" > "$output_file"
                elif [ "$extension" = "py" ]; then
                    python3 "$target_python_folder/$roll/main.py" < "$test_file" > "$output_file"
                fi

                answer_file="$answer_folder/ans$test_number.txt"
                diff_output=$(diff -q "$output_file" "$answer_file")

                if [ -z "$diff_output" ]; then
                    #echo "Test $test_number: Passed"
                    ((matched_tests++))
                else
                    #echo "Test $test_number: Failed"
                    ((failed_tests++))
                fi
            fi
        done
        echo "$roll,$language,$matched_tests,$failed_tests" >> "$result_file"
    fi

    
done
