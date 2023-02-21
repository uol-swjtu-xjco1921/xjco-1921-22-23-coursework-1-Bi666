#!/bin/bash

# TO USE THIS TEST SCRIPT

# FIRST USE ONLY - in terminal type: 
# chmod +x test.sh
# this ensures you have permission to run this script
# you should only need to do this once.

# You can then run with: 
# ./test.sh
# You can save your tests into a file using:
# ./tests.sh > name_of_file.txt
# you can edit which executables to run on line 90.

# we clean up object files and then recompile as standard
# this is because when you may be working across multiple devices
# your object files are not necessarily tranferable, and therefore
# you should always recompile.
make clean
make all

######### DO NOT EDIT THIS SECTION #########
# this section contains various things which the rest of the code relies on.
# If you want to edit anything here:
#       1) make a backup of this file first
#       2) read the comments and ensure you understand what you are changing


# setting up correct files for permission tests
# touch creates a new file (if it doesn't already exist)
# and chmod is used to set permissons
# -w means 'remove write permissions for all users'
# -r means 'remove read permissions for all users'
# we restore these at the end of the tests.
touch test_data/pgms/bad.out
chmod -w test_data/pgms/bad.out
chmod -r test_data/pgms/ascii/test.pgm

# keep track of score and total count of tests
total=0
score=0

# Usage: run_test $1 $2 $3 $4 $5
# Args: $1=executable name $2=executable arg 1 $3=executable arg 2 $4=expected return value $5=expected message

# This function takes in the arguments stated above and runs the given executable
# with the two specified arguments.
# the output code and string are captured
# and these are compared to the given expected code and string
# displaying a helpful message to the user
# and updating both the number of tests run and the number of passes
run_test () 
    { # run_test()
    # capture returned message
    message=$($1 $2 $3) 
    # run again (pipe to null so it doesn't display to user) for the output code
    echo "Test case: $1 $2 $3" 
    $1 $2 $3 > null 
    out=$?
    # increment total by 2 because 2 tests are run (code and message)
    total=$(( $total+2 ))

    # check output value against expected
    if [ $out == $4 ]
    then
        echo "PASSED [return value]: expected output value \"$4\" got $out"
        # if we passed, increment the score
        (( score++ ))
    else
        echo "FAILED [return value]: expected output value \"$4\" got $out"
    fi

    # check output message against expected
    if [ "$message" == "$5" ] 
    then
        echo "PASSED [output message]: expected printout message \"$5\" got \"$message\""
        (( score++ ))
    else
        echo "FAILED [output message]: expected printout message \"$5\" got \"$message\""
    fi

    } # end run_test()

######### YOU CAN EDIT BELOW THIS LINE #########



# you can remove or comment out any executables you don't want to test
# full list of executables: pgmEcho pgmComp etc.
# E.g. EXES = (pgmEcho pgmComp)
EXES=(pgmEcho)

# run all of the tests below for all executables given in 'EXES'
# inside this loop, the executable being run can be referred to by 'testExecutable'
for testExecutable in ${EXES[@]}
do 
    #relative path to the folder that contains the test images
    path="test_data/pgms/ascii/"
    #relative path to the folder that contains the expected outcomes
    oracle_path="test_data/pgms/output/"
    echo "-------------- TESTING $testExecutable --------------"

    # The order of these tests is based on when things should fail
    # this order is important!
    # For example, if I run ./pgmEcho tests/pgms/text/test.pgm
    # I should never get as far as opening the file since there is
    # and incorrect number of arguments.
    # Opening the file would be a waste of processing time and memory
    # because the command is invalid.

    # In many later tests, the 2nd argument may not exist.
    # You may find that you are getting 'Bad File Name (tmp)' if you have structured your
    # code incorrectly (i.e. reading the 2nd file before validating the data from
    # the 1st file). You should ensure that your code fails at the right point-
    # always finish reading and checking one file before you open the next.

    # most of these tests should be easy to understand but clarification comments have been
    # added for some. You can also examine the data in the tests/data subfolders

    # Spoofing no args using empty strings
    echo "Usage"
    run_test ./$testExecutable "" "" 0 "Usage: ./$testExecutable inputImage.pgm outputImage.pgm"
    
    # Note that although these files do not exist, the thing
    # which should cause the failure is that there are 3 arguments
    # NOT that the filenames are incorrect.
    echo ""
    echo "Bad Args"
    run_test ./$testExecutable "1 2" "3" 1 "ERROR: Bad Arguments"
    
    # Neither '1' nor '2' exists, but it should fail on '1' because
    # you should be opening, validating, reading and closing
    # before you access file 2.
    echo ""
    echo "Bad Filename"
    run_test ./$testExecutable "1" "2" 2 "ERROR: Bad File Name (1)"
    
    # we have deliberately removed read permissions 
    # for this file using chmod
    echo ""
    echo "Bad Permissions"
    filename="test.pgm"
    full_path=$path$filename
    run_test ./$testExecutable $full_path "2" 2 "ERROR: Bad File Name ($full_path)"
    
    echo ""
    echo "Bad Magic Number"
    filename="bad_magic_number.pgm"
    full_path=$path$filename
    run_test ./$testExecutable $full_path "2" 3 "ERROR: Bad Magic Number ($full_path)"

    echo ""
    echo "Bad Dimensions (big)"
    filename="bad_dimensions.pgm"
    full_path=$path$filename
    run_test ./$testExecutable $full_path "tmp" 4 "ERROR: Bad Dimensions ($full_path)"

    ### Functionality Tests ###

    # to test the functionality, we need to check which 'class' of executable
    # we are working with - echo, comp or conversion. We will do this by looking
    # at substrings from the executable name.

    # note that where diff is used you may see slight differences if you have linebreaks
    # or spaces used differently. Linebreaks at the end of the file are bad practice BUT
    # will be accounted for in the final test.

    # if we have an Echo function:
    if [[ ${testExecutable:3:4} == "Echo" ]]
    then
        # To test echo we will:
        #    - check for the right code and message
        #    - and check that the files match using the diff tool
        echo ""
        echo "Testing $testExecutable Functionality"
        filename="slice0a.pgm"
        full_path=$path$filename
	oracle=$oracle_path"pgmEcho/"$filename
        run_test ./$testExecutable $full_path "tmp" 0 "ECHOED"
        D=$(diff $oracle tmp)
        if [[ $D != "" ]]
        then
            echo "FILES ARE DIFFERENT"
            echo $D
        else
            echo "FILES ARE IDENTICAL"
        fi
    fi
done

###### DO NOT REMOVE - restoring permissions
# git will be unable to deal with files when we don't have permissions
# so to prevent you having to deal with untracked files, we will restore
# permissions after running the tests.

# chmod +(w / r) means 'give all users write / read permission for the file'
chmod +w test_data/pgms/bad.out
chmod +r test_data/pgms/ascii/test.pgm
###### YOU CAN EDIT BELOW THIS POINT

# the run_test function has been incrementing both a counter for how many tests have run
# and a counter for passes. We will display this (along with a message for clarity).
echo "------------------------------------------------------------------------------"
echo "You passed $score out of $total"
echo "IMPORTANT: this is a count of tests passed - it is not your final grade."
echo "------------------------------------------------------------------------------"


# OPTIONAL - neither of these will affect your tests
# Many programmers prefer to work from a relatively 'tidy' directory
# and so will ensure any temp files created by the tests are removed
# and any object files are removed.

# remove the files we created during the tests
rm tmp null
# and run make clean to remove object files
make clean
