#!/bin/bash

PROJDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"

cd $PROJDIR;

DIFF=$(which diff)
GREP=$(which grep)
VALGRIND=$(which valgrind)

AAF_PATH="${PROJDIR}/test/aaf"
BIN_PATH="${PROJDIR}/build/bin"
EXPECTED_OUTPUT_PATH="${PROJDIR}/test/expected"
OUTPUT_PATH="${PROJDIR}/test/output"
MEDIA_PATH="${PROJDIR}/test/res"
AAFINFO_CMD="${BIN_PATH}/AAFInfo --aaf-summary --aaf-essences --aaf-clips --pos-format samples --no-color --verb 0 --media-location \"${AAF_PATH}/res\"" # --media-location \"${AAF_PATH}/res\"

VALGRIND_CMD="$VALGRIND --error-exitcode=1 --track-origins=yes --leak-check=full --show-leak-kinds=all"
# DIFF_CMD="git diff --unified=0 --word-diff=color --minimal"
DIFF_CMD="$DIFF"

OPT_RUN_FROM_CMAKE=0

if [ x"$1" = x"--run-from-cmake" ]; then
	OPT_RUN_FROM_CMAKE=1
fi

RC=0



test() {

  printf " [....] [....] \033[38;5;81m$1\x1b[0m"

	aafFile="$AAF_PATH/$1"
	expectedAAFInfoOutputFile="$EXPECTED_OUTPUT_PATH/$1.expected"

	valgrindOutputFile="$OUTPUT_PATH/$1.valgrind"
	aafInfoOutputFile="$OUTPUT_PATH/$1.aafinfo"
	aafInfoDiffFile="$OUTPUT_PATH/$1.aafinfodiff"

	RES_VALGRIND_ERROR=0
	RES_AAFINFO_ERROR=0

  if [ ! -f "$aafFile" ]; then
    printf " : \033[38;5;124mMissing AAF file \"$aafFile\"\x1b[0m\n"
		return;
  fi

  if [ ! -f "$expectedAAFInfoOutputFile" ]; then
    printf " : \033[38;5;124mMissing expected-output file \"$expectedAAFInfoOutputFile\"\x1b[0m\n"
		return;
  fi


	cmd="$VALGRIND_CMD --quiet --log-file=$valgrindOutputFile $AAFINFO_CMD $2 --log-file $aafInfoOutputFile $aafFile"
	$cmd 2>&1>/dev/null

	if [ $? -ne 0 ]; then
		RES_VALGRIND_ERROR=1
  fi

	DIFFED=$($DIFF_CMD "$expectedAAFInfoOutputFile" "$aafInfoOutputFile" | grep -vE '(\+\+\+)|(\-\-\-)|@@') #- -word-diff-regex=.

  if [ -n "$DIFFED" ]; then
		RES_AAFINFO_ERROR=1
		printf "$DIFFED\n" > "$aafInfoDiffFile"
  fi


	printf "\r "

	if [ $RES_AAFINFO_ERROR -eq 1 ]; then
		printf "[\033[38;5;124mdiff\x1b[0m] "
	else
		printf "[\x1b[92mdiff\x1b[0m] "
	fi

	if [ $RES_VALGRIND_ERROR -eq 1 ]; then
		printf "[\033[38;5;124mleak\x1b[0m] "
	else
		printf "[\x1b[92mleak\x1b[0m] "
	fi

	printf "\033[38;5;81m$1\x1b[0m\n"


	if [ $RES_AAFINFO_ERROR -eq 1 ] || [ $RES_VALGRIND_ERROR -eq 1 ]; then
		printf "   :: Test command : $cmd\n"
		RC=1
	fi

	if [ $RES_AAFINFO_ERROR -eq 1 ]; then
		printf "   :: Expected     : $expectedAAFInfoOutputFile\n"
		printf "   :: AAFInfo      : $aafInfoOutputFile\n"
		printf "   :: Diff         : $aafInfoDiffFile\n"
	# else
	# 	rm "$aafInfoOutputFile"
	# 	rm "$aafInfoDiffFile"
	fi

	if [ $RES_VALGRIND_ERROR -eq 1 ]; then
		printf "   :: Valgrind     : $valgrindOutputFile\n"
	# else
	# 	rm "$valgrindOutputFile"
	fi

	if [ $RES_AAFINFO_ERROR -eq 1 ] || [ $RES_VALGRIND_ERROR -eq 1 ]; then
		printf "\n"

		if [ $OPT_RUN_FROM_CMAKE -eq 1 ]; then
			exit 1
		fi
	fi
}



update() {

	if [ $OPT_RUN_FROM_CMAKE -eq 1 ]; then
		printf "Guard: Can't update .expected files from cmake !"
		exit 1
	fi

  printf " [....] \033[38;5;130m$1\x1b[0m"

	aafFile="$AAF_PATH/$1"
  expectedAAFInfoOutputFile="$EXPECTED_OUTPUT_PATH/$1.expected"

  if [ ! -f "$aafFile" ]; then
    printf " : \033[38;5;124mMissing AAF file \"$aafFile\"\x1b[0m\n\n"
    exit 1
  fi

  $AAFINFO_CMD $2 --log-file "$expectedAAFInfoOutputFile" "$aafFile" 1>/dev/null
	printf "\r [\x1b[92m ok \x1b[0m] \033[38;5;130m$1\x1b[0m\n"
}



echo

test "PT_WAV_External.aaf"                "--media-location ${MEDIA_PATH}"
test "PT_WAV_External_sub_directory.aaf"  "--samplerate 44100"
test "PT_WAV_External_same_directory.aaf" "--samplerate 44100"
test "PT_AIFF_External.aaf"
test "PT_MXF_External.aaf"
test "PT_PCM_Internal.aaf"                "--samplerate 44100"
test "DR_MP3_External.aaf"
test "DR_Mono_Clip_Positioning.aaf"
test "DR_Stereo_Clip_Positioning.aaf"     "--samplerate 48000"
test "DR_Empty.aaf"

echo

exit $RC
