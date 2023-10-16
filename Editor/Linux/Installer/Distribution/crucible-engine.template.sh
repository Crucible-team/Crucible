#!/bin/sh

echo "Starting Crucible Engine Editor"

cd @CMAKE_INSTALL_PREFIX@/@EDITOR_INSTALL_FOLDER@
exec ./JOE $@
