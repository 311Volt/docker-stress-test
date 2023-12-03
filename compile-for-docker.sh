#!/bin/bash
docker build -t docker-stress-test-builder docker-builder
docker run --mount type=bind,source="$(pwd)",target=/root/workdir docker-stress-test-builder