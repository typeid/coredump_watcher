*** Settings ***
Resource   coredump_watcher.resource
Library    String
Library    OperatingSystem

Test Setup          Set Up Test
Test Teardown       Tear Down Test

*** Variables ***
${TEST_OUTPUT_FOLDER}=      testoutput
${TEST_FILE_FOLDER}=        testfiles
${WATCH_DIRECTORY}=    ${TEST_OUTPUT_FOLDER}

${folder1_origin}=    ${TEST_FILE_FOLDER}/folder1
${folder3_origin}=    ${TEST_FILE_FOLDER}/folder3
${logfile1_origin}=   ${TEST_FILE_FOLDER}/logfile1


*** Keywords ***
Set Up Test
    Run Process    bash    -c    mkdir ${TEST_OUTPUT_FOLDER}

    Start Coredump Watcher    watch_directory=${WATCH_DIRECTORY}    archive_files=${logfile1_origin}
    ...    archive_folders=${folder1_origin},${folder3_origin}
    ...    archive_output_directory=${TEST_OUTPUT_FOLDER}

Tear Down Test
    Process Should Be Running    ${PROCESS_HANDLE}
    ${result}=    Terminate Process    ${PROCESS_HANDLE}
    Log           ${result.stdout}
    Log           ${result.stderr}

    Remove Directory    ${TEST_OUTPUT_FOLDER}    recursive=True

Create Trigger File
    ${random_string}=    Generate Random String      10      [NUMBERS]abcdef

    ${trigger_file_name}=     Set Variable    core.ServiceName.${random_string}.lz4

    Run Process    bash    -c    echo "test" > ${WATCH_DIRECTORY}/${trigger_file_name}

    [return]     ${trigger_file_name}

Files Should Be Same
    [Arguments]    ${file1}     ${file2}   

    ${result}=     Run Process    bash    -c    diff ${file1} ${file2}
    Should Be Equal    ${result.rc}    ${0}
    Log    ${result.stdout}
    Log    ${result.stderr}

*** Test Cases ***
Files Are As Expected
    ${trigger_file_name}=    Create Trigger File

    # Check tar integrity of created file
    Run Process    bash    -c    tar -tf ${TEST_OUTPUT_FOLDER}/*.tar > /dev/null

    # Extract
    ${extraction_folder}=     Set Variable    ${TEST_OUTPUT_FOLDER}/extracted
    Create Directory     ${extraction_folder}
    Run Process    bash    -c    tar -xf ${TEST_OUTPUT_FOLDER}/*.tar --directory ${extraction_folder}

    # Compare extracted files with expected files
    Files Should Be Same     ${extraction_folder}/testfiles/folder1                  ${folder1_origin}
    Files Should Be Same     ${extraction_folder}/testfiles/folder3                  ${folder3_origin}
    Files Should Be Same     ${extraction_folder}/testfiles/logfile1                 ${logfile1_origin}
    Files Should Be Same     ${extraction_folder}/testoutput/${trigger_file_name}    ${TEST_OUTPUT_FOLDER}/${trigger_file_name}

    ${disk_usage_report}=    Get File     ${extraction_folder}/disk_usage_report

    # Check that the disk usage report contains data matching a df -T call
    ${contains}=  Evaluate   "Used Available Use% Mounted on" in """${disk_usage_report}"""
    Should Be True    ${contains}

    # Check that file count in the extracted folder matches the expected count
    # Should contain the following 9 files/folders:
    # disk_usage_report
    # testfiles/logfile1
    # testfiles/folder1/
    # testfiles/folder1/folder2/
    # testfiles/folder1/folder2/file2
    # testfiles/folder1/file1
    # testfiles/folder3/
    # testfiles/folder3/loremipsum
    ${result}=     Run Process    bash    -c    tar -tf ${TEST_OUTPUT_FOLDER}/*.tar | wc -l
    Should Be Equal     ${result.stdout}    9 

   


