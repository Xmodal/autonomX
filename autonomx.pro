TEMPLATE = subdirs

# Silent a warning on macOS:
CONFIG += sdk_no_version_check

# autonomx
sub_autonomx.subdir = autonomx
sub_autonomx.target = sub_autonomx
SUBDIRS += autonomx

# test_autonomx:
sub_test_autonomx.subdir = test_autonomx
sub_test_autonomx.target = sub_test_autonomx
sub_test_autonomx.depends += sub_autonomx
SUBDIRS += test_autonomx

