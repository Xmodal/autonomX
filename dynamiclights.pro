TEMPLATE = subdirs

# Silent a warning on macOS:
CONFIG += sdk_no_version_check

# DynamicLights:
sub_DynamicLights.subdir = DynamicLights
sub_DynamicLights.target = sub_DynamicLights
SUBDIRS += DynamicLights

# test_DynamicLights:
sub_test_DynamicLights.subdir = test_DynamicLights
sub_test_DynamicLights.target = sub_test_DynamicLights
sub_test_DynamicLights.depends += sub_DynamicLights
SUBDIRS += test_DynamicLights

