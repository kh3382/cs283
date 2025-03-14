#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

setup() {
    ./dsh -s 2>/dev/null & 
    server_pid=$!
    sleep 1 
}

teardown() {
    kill $server_pid
    wait $server_pid 2>/dev/null || true
}

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Basic connection test" {
    run ./dsh -c <<EOF                
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Echo command test" {
    run ./dsh -c <<EOF                
echo hello world
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>helloworlddsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Built-in command exit" {
    run ./dsh -c <<EOF                
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Built-in command stop-server test" {
    run ./dsh -c <<EOF                
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Built-in cd command test" {
    # for testing pwd command
    expected_pwd=&(cd .. && pwd)
    stripped_expected_pwd=$(echo "&expected_pwd" | tr -d '[:space:]')

    run ./dsh -c <<EOF                
cd ..
pwd
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> Expected contains: ${stripped_expected_pwd}"

    # Check exact match
    [[ "$stripped_output" == *"$stripped_expected_pwd"* ]]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Simple pipe test" {
    run ./dsh -c <<EOF
printf "banana\napple\ncarrot\n" | sort | head -n 1 
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>appledsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Multiple commands pipeline test" {
    run ./dsh -c <<EOF                
echo "a b c" | tr ' ' '\n' | grep b
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>bdsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Invalid command test" {
    run ./dsh -c <<EOF
foo
exit
EOF

    # filtering unwanted output
    filtered_output=$(echo "$output" | grep -Ev 'socket server mode|-> Single-Threaded Mode')

    # Strip all whitespace (spaces, tabs, newlines)
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching (cleaned)
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>rdsh-error:commandexecutionerrordsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Filtered Output: $filtered_output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Multiple clients test" {
    # input files for each client
    echo "ls" > client1_input.txt
    echo "exit" >> client1_input.txt

    echo "cd" > client2_input.txt
    echo "exit" >> client2_input.txt

    # run client 1 and 2
    ./dsh -c < client1_input.txt > client1_output.txt &
    pid1=$!

    ./dsh -c < client2_input.txt > client2_output.txt &
    pid2=$!

    # wait for both clients 
    wait $pid1
    wait $pid2

    output1=$(<client1_output.txt)
    output2=$(<client2_output.txt)

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output1=$(echo "$output1" | tr -d '[:space:]')
    stripped_output2=$(echo "$output2" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_part="socketclientmode:addr:127.0.0.1:1234dsh4>"


    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Client 1 Output: $output1"
    echo "Client 2 Output: $output2"
    echo "${stripped_output1} -> Expected contains ${expected_part}"
    echo "${stripped_output2} -> Expected contains ${expected_part}"

    # Check match
    [[ "$stripped_output1" == *"$expected_part"* ]]
    [[ "$stripped_output2" == *"$expected_part"* ]]

    # clean up
    rm -f client1_output.txt client2_output.txt client1_input.txt client2_input.txt
}

@test "Invalid command in pipeline test" {
    run ./dsh -c <<EOF                
cat rshlib.h | foo | sort
exit
EOF

    # filter out unwanted output
    filtered_output=$(echo "$output" | grep -Ev 'socket server mode|-> Single-Threaded Mode')

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>rdsh-error:commandexecutionerrorcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Filtered Output: $filtered_output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Multi-threaded server test" {
    # kill any previous server
    pkill -f "./dsh -s -x" 2>/dev/null || true
    sleep 1

    # start server in threaded mode
    ./dsh -s -x 2>/dev/null &
    server_pid=$!
    sleep 2

    # client 1
    ./dsh -c > client1_output.txt <<EOF &           
echo client1
exit
EOF
    pid1=$!

    # client 2
    ./dsh -c > client2_output.txt <<EOF &
echo client2
exit
EOF
    pid2=$!

    wait $pid1
    wait $pid2

    # stop server
    ./dsh -c > /dev/null <<EOF
stop-server
EOF

    sleep 1

    # kill if still running
    if kill -0 $server_pid 2>/dev/null; then
        kill $server_pid
        wait $server_pid 2>/dev/null || true
    fi

    # save outputs
    output1=$(<client1_output.txt)
    output2=$(<client2_output.txt)

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output1=$(echo "$output1" | tr -d '[:space:]')
    stripped_output2=$(echo "$output2" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_client1="socketclientmode:addr:127.0.0.1:1234dsh4>client1dsh4>serverappearedtoterminate-exitingcmdloopreturned0"
    expected_client2="socketclientmode:addr:127.0.0.1:1234dsh4>client2dsh4>serverappearedtoterminate-exitingcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured Client 1 Output: $output1"
    echo "Captured Client 2 Output: $output2"
    echo "Exit Status: $status"
    echo "${stripped_output1} -> ${expected_client1}"
    echo "${stripped_output2} -> ${expected_client2}"

    # Check exact match
    [ "$stripped_output1" = "$expected_client1" ]
    [ "$stripped_output2" = "$expected_client2" ]

    # cleanup
    rm -f client1_output.txt client2_output.txt
}