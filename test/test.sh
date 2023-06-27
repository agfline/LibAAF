#!/bin/bash

PROJDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"

cd $PROJDIR;

AAF_PATH="${PROJDIR}/test/aaf"
BIN_PATH="${PROJDIR}/bin"
EXPECTED_OUTPUT_PATH="${PROJDIR}/test/expected"
TEST_CMD="${BIN_PATH}/AAFInfo --aaf-summary --aaf-essences --aaf-clips --pos-format hms --media-location \"${AAF_PATH}/res\""

DIFF=$(which diff)
GREP=$(which grep)


test() {

  printf " [....] [....] $1"

  testOutput=$(mktemp)
  expectedOutput="$EXPECTED_OUTPUT_PATH/$2"
  aafFile="$AAF_PATH/$3"

  if [ ! -f "$aafFile" ]; then
    printf " : \033[38;5;124mMissing AAF file \"$aafFile\"\x1b[0m\n"
    echo ""
    exit 1
  fi

  if [ ! -f "$expectedOutput" ]; then
    printf " : \033[38;5;124mMissing expected-output file \"$expectedOutput\"\x1b[0m\n"
    echo ""
    exit 1
  fi

  # $TEST_CMD "$AAF_PATH/$FILE" 2>/dev/null | sed -e '1,4d' > $expectedOutput

  $TEST_CMD "$aafFile" 2>/dev/null | sed -e '1,4d' > $testOutput

  DIFFED=$(git diff --unified=0 --word-diff=color --minimal $expectedOutput $testOutput | sed -e '1,3d' | grep -vE '(\+\+\+)|(\-\-\-)|@@') #- -word-diff-regex=.

  rm -f $testOutput

  if [ -z "$DIFFED" ]; then
    printf "\r [\x1b[92mdiff\x1b[0m] [....] $1"
  else
    printf "\r [\033[38;5;124mdiff\x1b[0m] [....] $1\n\n"
    printf "   Test command was : $TEST_CMD $aafFile\n\n"
    echo "$DIFFED"
    echo ""
    exit 1
  fi


  valgrind --error-exitcode=1 --log-file="$testOutput" --track-origins=yes --leak-check=full --show-leak-kinds=all $TEST_CMD "$aafFile" > /dev/null 2>&1

  if [ $? -eq 0 ]; then
    rm -f $testOutput
    printf "\r [\x1b[92mdiff\x1b[0m] [\x1b[92mleak\x1b[0m] $1\n"
  else
    printf "\r [\x1b[92mdiff\x1b[0m] [\033[38;5;124mleak\x1b[0m] $1\n\n"
    cat $testOutput;
    rm -f $testOutput
    echo ""
    exit 1
  fi
}

echo

test "Davinci Resolve 18.5 Full Timeline"   "resolve_18.5.expected"           "Resolve 18.5.aaf"
test "LogicPro 10.7.4.5614 Full Timeline"   "logicpro_10.7.4.5614.expected"   "LogicPro 10.7.4.5614.AAF"

echo
