#!/bin/bash

pushd $(dirname $0) >/dev/null
export secure_dir=$(pwd)   # always points to the folder where the script is
                           # whatever from where you launch it and export it
                           # as environment variable to use it within the
                           # doxygen configuration
popd >/dev/null


doxygen $secure_dir/doxygen_config/doxygen.cfg > /dev/null

printf "Doxygen documentation generated.\n"
printf "Launch $secure_dir/html/index.html into your favorite browser.\n"

