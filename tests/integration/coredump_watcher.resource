*** Settings ***
Library     Process

*** Variables ***
${PROCESS_HANDLE}=      ${EMPTY}

*** Keywords ***
Start Coredump Watcher
    [Arguments]    ${watch_directory}    ${archive_files}    ${archive_folders}    ${archive_output_directory}    ${archive_coredump}=true

    ${process}=    Start Process    ../../build/coredump_watcher
    ...    --watch_directory    ${watch_directory}
    ...    --archive_files    ${archive_files}
    ...    --archive_folders    ${archive_folders}
    ...    --archive_output_directory    ${archive_output_directory}
    ...    --archive_coredump        ${archive_coredump}

    Set Global Variable    ${PROCESS_HANDLE}    ${process}
