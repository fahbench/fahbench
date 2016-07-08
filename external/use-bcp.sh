#!/bin/bash

set -e

# Use boost's `bcp` program to copy only what we need
# Set environment variable BCP to the location of the bcp tool
# Set environment variable BOOST_ROOT to be location of boost

boost_dir='boost'
bcpp="$BCP --boost=$BOOST_ROOT"

mkdir -p $boost_dir
$bcpp \
    boost/program_options.hpp \
    boost/filesystem.hpp \
    boost/function.hpp \
    boost/format.hpp \
    boost/property_tree/ptree.hpp \
    boost/property_tree/json_parser.hpp \
    $boost_dir
