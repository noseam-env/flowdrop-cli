# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL

function(app_parse_version file)
    #file(STRINGS ${file} lines)
    file(READ ${file} file_content)
    string(STRIP "${file_content}" trimmed_content)
    set(app_version ${trimmed_content} PARENT_SCOPE)
endfunction()
