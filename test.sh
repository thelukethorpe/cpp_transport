#!/bin/bash

cd build || exit 1

./cpp_transport_tests --output-on-failure
