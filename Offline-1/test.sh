#!/bin/bash

# Check if the required arguments are provided
if [ "$#" -lt 4 ]; then
    echo "Usage: $0 <submissions_folder> <targets_folder> <tests_folder> <answers_folder> [-v] [-noexecute]"
    exit 1 
fi

# Retrieve the arguments
source_folder="$1"
target_folder="$2"
test_folder="$3"
answer_folder="$4"

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
echo "StudentID,Language,MatchedTests,FailedTests" > "$result_file"
fi

for subdirectory in "$source_folder"/*; do
    if [[ $subdirectory =~ _assignsubmission_file_([0-9]{7})\.zip ]]; then
        roll="${BASH_REMATCH[1]}"
        #echo "Roll: $roll"
        if $verbose; then
            echo "Organizing file of $roll"
        fi
    else
        echo "Unable to extract roll from folder name."
        exit 1
    fi

    temp_dir=$(mktemp -d)
    unzip -q "$subdirectory" -d "$temp_dir"

    code_file=$(find "$temp_dir" -type f \( -name "*.c" -o -name "*.java" -o -name "*.py" \))

    if [[ $code_file == *.c ]]; then
        language="c"
        extension="c"
    elif [[ $code_file == *.java ]]; then
        language="java"
        extension="java"
    elif [[ $code_file == *.py ]]; then
        language="python"
        extension="py"
    else
        language="unknown"
        extension=""
    fi

    if [[ $language == "c" ]]; then
        new_file="main.c"
        roll_folder="$target_c_folder/$roll"
        mkdir -p "$target_c_folder/$roll"
        mv "$code_file" "$target_c_folder/$roll/$new_file"
    elif [[ $language == "java" ]]; then
        new_file="main.java"
        roll_folder="$target_java_folder/$roll"
        mkdir -p "$target_java_folder/$roll"
        mv "$code_file" "$target_java_folder/$roll/$new_file"
    elif [[ $language == "python" ]]; then
        new_file="main.py"
        roll_folder="$target_python_folder/$roll"
        mkdir -p "$target_python_folder/$roll"
        mv "$code_file" "$target_python_folder/$roll/$new_file"
    else
        #echo "Unsupported language: $language"
        exit 1
    fi


    if ! $noexecute; then
        if [ "$language" = "c" ]; then
            gcc "$target_c_folder/$roll/$new_file" -o "$target_c_folder/$roll/main.out"
        elif [ "$language" = "java" ]; then
            javac "$target_java_folder/$roll/$new_file" -d "$target_java_folder/$roll/"
        fi

        matched_tests=0
        failed_tests=0

        for test_file in "$test_folder"/*; do
            if [ -f "$test_file" ]; then
                test_filename=$(basename "$test_file")
                test_number="${test_filename#test}"
                test_number="${test_number%.txt}"
                output_file="$roll_folder/out$test_number.txt"

                # Create the output file
                touch "$output_file"

                # Run the code with the test case and store the output
                if $verbose; then
                    echo "Executing file of $roll"
                fi
                if [ "$extension" = "c" ]; then
                    "$target_c_folder/$roll/main.out" < "$test_file" > "$output_file"
                elif [ "$extension" = "java" ]; then
                    java -cp "$target_java_folder/$roll/" Main < "$test_file" > "$output_file"
                elif [ "$extension" = "py" ]; then
                    python3 "$target_python_folder/$roll/main.py" < "$test_file" > "$output_file"
                fi

                # Compare the output with the corresponding answer file
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
