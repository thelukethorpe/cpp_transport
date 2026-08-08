#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
	echo "Error: Missing required arguments. Usage: $0 <version> <commit_message>" >&2
	exit 1
fi

VERSION=${1}
COMMIT_MESSAGE=${2}

echo "Releasing version: $VERSION"

git add .
git commit -m "v$VERSION - $COMMIT_MESSAGE"
git push origin main
git tag -a v$VERSION -m "cpp_transport v$VERSION"
git push origin v$VERSION
