#!/bin/python3

import io
import os
import sys
import subprocess
import difflib
import argparse
import hashlib

errorCounts = 0

parser = argparse.ArgumentParser()
parser.add_argument('--aaftool')
parser.add_argument('--update', action='store_true')
parser.add_argument('--wine', action='store_true')
parser.add_argument('--run-from-cmake', action='store_true')
args = parser.parse_args()

if sys.platform.startswith('win32'):
	DIR_SEP = "\\"
	BIN_AAFTOOL = "aaftool.exe"
else:
	DIR_SEP = "/"
	BIN_AAFTOOL = "aaftool"


TEST_DIR = os.path.dirname(os.path.abspath(__file__))
TEST_AAF_DIR      = TEST_DIR + DIR_SEP + "aaf"
TEST_EXPECTED_DIR = TEST_DIR + DIR_SEP + "expected"
TEST_MEDIA_DIR    = TEST_DIR + DIR_SEP + "res"
TEST_OUTPUT_PATH  = TEST_DIR + DIR_SEP + "output"

PROJ_DIR = os.path.dirname(TEST_DIR)
BIN_DIR  = PROJ_DIR + DIR_SEP + "build" + DIR_SEP + "bin"

BIN_GREP = ""
BIN_DIFF = ""
BIN_VALGRIND = ""

if args.aaftool != None:
	AAFTOOL_CMD = "\"" + args.aaftool + "\" --aaf-summary --aaf-essences --aaf-clips --pos-format samples --show-automation --show-metadata"
else:
	if args.wine:
		AAFTOOL_CMD = "wine " + BIN_DIR + DIR_SEP + BIN_AAFTOOL + ".exe --aaf-summary --aaf-essences --aaf-clips --pos-format samples --show-automation --show-metadata"
	else:
		AAFTOOL_CMD = BIN_DIR + DIR_SEP + BIN_AAFTOOL + " --aaf-summary --aaf-essences --aaf-clips --pos-format samples --show-automation --show-metadata"


GREP_CMD = ""
DIFF_CMD = ""
VALGRIND_CMD = ""


# prepare ANSI colors for windows

if not __import__("sys").stdout.isatty():
	ANSI_COLOR_RED    = ""
	ANSI_COLOR_CYAN   = ""
	ANSI_COLOR_GREEN  = ""
	ANSI_COLOR_ORANGE = ""
	ANSI_COLOR_END    = ""

else:
	ANSI_COLOR_RED    = "\x1b[38;5;124m"
	ANSI_COLOR_CYAN   = "\x1b[38;5;81m"
	ANSI_COLOR_GREEN  = "\x1b[92m"
	ANSI_COLOR_ORANGE = "\x1b[38;5;130m"
	ANSI_COLOR_END    = "\x1b[0m"

	# set Windows console in VT mode
	if __import__("platform").system() == "Windows":
		kernel32 = __import__("ctypes").windll.kernel32
		kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7)
		del kernel32


if sys.platform.startswith('linux'):
	data = subprocess.run( "which grep", capture_output=True, shell=True, text=True )
	BIN_GREP = data.stdout.rstrip('\n')

	if BIN_GREP == "":
		print("Error: can't locate grep")
		exit(1)

	data = subprocess.run( "which diff", capture_output=True, shell=True, text=True )
	BIN_DIFF = data.stdout.rstrip('\n')

	if BIN_DIFF == "":
		print("Error: can't locate grep")
		exit(1)

	DIFF_CMD = BIN_DIFF

	if not args.wine:
		data = subprocess.run( "which valgrind", capture_output=True, shell=True, text=True )
		BIN_VALGRIND = data.stdout.rstrip('\n')

		if BIN_VALGRIND == "":
			print( "Error: can't locate valgrind" )
			exit(1)

		VALGRIND_CMD = BIN_VALGRIND + " --error-exitcode=1 --track-origins=yes --leak-check=full --show-leak-kinds=all"

elif sys.platform.startswith('win32'):
	BIN_DIFF = 'fc.exe '
	DIFF_CMD = BIN_DIFF + ' /U'




def test( aafFileName, aaftoolAddCmd ):

	if args.run_from_cmake and args.update:
		return

	if args.update:
		return update( aafFileName, aaftoolAddCmd )

	global errorCounts
	global VALGRIND_CMD

	if BIN_VALGRIND != "":
		print( " [....] [....] ", end="" )
	else:
		print( " [....] ", end="" )

	print(ANSI_COLOR_CYAN + aafFileName + ANSI_COLOR_END, end="")
	sys.stdout.flush()

	aafFile = TEST_AAF_DIR + DIR_SEP + aafFileName;
	expectedAaftoolOutputFile = TEST_EXPECTED_DIR + DIR_SEP + aafFileName + ".expected"

	valgrindOutputFile = TEST_OUTPUT_PATH + DIR_SEP + aafFileName + ".valgrind"
	aaftoolOutputFile  = TEST_OUTPUT_PATH + DIR_SEP + aafFileName + ".aaftool"
	aaftoolDiffFile    = TEST_OUTPUT_PATH + DIR_SEP + aafFileName + ".aaftooldiff"

	if os.path.exists(valgrindOutputFile):
		os.remove(valgrindOutputFile)
	if os.path.exists(aaftoolOutputFile):
		os.remove(aaftoolOutputFile)
	if os.path.exists(aaftoolDiffFile):
		os.remove(aaftoolDiffFile)

	if not os.path.isfile(aafFile):
		print( " : " + ANSI_COLOR_RED + "Missing AAF file \""+aafFile+"\"" + ANSI_COLOR_END )
		errorCounts+=1
		return

	if not os.path.isfile(expectedAaftoolOutputFile):
		print( " : " + ANSI_COLOR_RED + "Missing expected-output file \"" + expectedAaftoolOutputFile + "\"" + ANSI_COLOR_END )
		errorCounts+=1
		return


	valgrindError = False
	aaftoolError = False

	valgrindCmd = VALGRIND_CMD

	if valgrindCmd != "":
		valgrindCmd += " --quiet --log-file=\"" + valgrindOutputFile + "\" "

	testCmd = valgrindCmd + AAFTOOL_CMD + " " + aaftoolAddCmd + " \"" + aafFile + "\" --verb 0 --no-color --relative-path --log-file \"" + aaftoolOutputFile + "\""

	proc = subprocess.run(testCmd, capture_output=True, shell=True, text=True, encoding="utf-8")

	if proc.returncode != 0:
		if valgrindCmd != "":
			valgrindError = True

	if not os.path.isfile(aaftoolOutputFile):
		aaftoolError = True;


	if not aaftoolError:
		f1 = open( expectedAaftoolOutputFile, 'r', encoding="utf-8", errors='ignore' ).readlines()
		f2 = open( aaftoolOutputFile, 'r', encoding="utf-8", errors='ignore' ).readlines()

		diff = difflib.unified_diff( f1, f2, "expected", "test-output" )

		fp = open( aaftoolDiffFile, "w", encoding="utf-8" )
		fp.writelines(diff)
		fp.close()

		diff = difflib.unified_diff( f1, f2, "expected", "test-output" )

		if len(list(diff)):
			aaftoolError = True;


	print( "\r ", end="" )

	if aaftoolError:
		print( "[" + ANSI_COLOR_RED + "diff" + ANSI_COLOR_END + "] ", end="" )
	else:
		print( "[" + ANSI_COLOR_GREEN + "diff" + ANSI_COLOR_END + "] ", end="" )


	if BIN_VALGRIND != "":
		if valgrindError:
			print( "[" + ANSI_COLOR_RED + "leak" + ANSI_COLOR_END + "] ", end="" )
		else:
			print( "[" + ANSI_COLOR_GREEN + "leak" + ANSI_COLOR_END + "] ", end="" )


	print( ANSI_COLOR_CYAN + aafFileName + "" + ANSI_COLOR_END )


	if valgrindError or aaftoolError:
		print( "   :: Test command : " + testCmd )
		errorCounts+=1

	if aaftoolError:
		print( "   :: expected log : " + expectedAaftoolOutputFile )
		print( "   ::     test log : " + aaftoolOutputFile )
		print( "   :: log diff     : " + aaftoolDiffFile )

	if valgrindError:
		print( "   :: valgrind log : " + valgrindOutputFile )

	# if errorCounts and args.run_from_cmake:
	# 	sys.exit(errorCounts);



def extract( aafFileName, aaftoolAddCmd, fileArray ):

	if args.update:
		return

	global errorCounts
	global VALGRIND_CMD

	if BIN_VALGRIND != "":
		print( " [....] [....] ", end="" )
	else:
		print( " [....] ", end="" )

	print( ANSI_COLOR_CYAN + aafFileName + ANSI_COLOR_END, end="" )
	sys.stdout.flush()

	aafFile = TEST_AAF_DIR + DIR_SEP + aafFileName;
	expectedAaftoolOutputFile = TEST_EXPECTED_DIR + DIR_SEP + aafFileName + ".expected"

	valgrindOutputFile = TEST_OUTPUT_PATH + DIR_SEP + aafFileName + ".valgrind"

	if os.path.exists(valgrindOutputFile):
		os.remove(valgrindOutputFile)

	for file in fileArray:
		filepath = TEST_OUTPUT_PATH + DIR_SEP + file[1]
		if os.path.exists(filepath):
			os.remove(filepath)

	if not os.path.isfile(aafFile):
		print( " : " + ANSI_COLOR_RED + "Missing AAF file \""+aafFile+"\"" + ANSI_COLOR_END )
		errorCounts+=1
		return


	valgrindError = False
	hashMismatch  = False

	valgrindCmd = VALGRIND_CMD

	if valgrindCmd != "":
		valgrindCmd += " --quiet --log-file=\""+valgrindOutputFile+"\" "

	testCmd = valgrindCmd + AAFTOOL_CMD + " " + aaftoolAddCmd + " --extract-path " + TEST_OUTPUT_PATH + " \"" + aafFile + "\""

	proc = subprocess.run( testCmd, capture_output=True, shell=True, text=True )

	if proc.returncode != 0:
		if valgrindCmd != "":
			valgrindError = True


	for file in fileArray:
		validhash = file[0]
		filepath  = TEST_OUTPUT_PATH + DIR_SEP + file[1]

		if not os.path.isfile(filepath):
			print( " : " + ANSI_COLOR_RED + "Missing extracted file \"" + aafFile + "\"" + ANSI_COLOR_END )
			hashMismatch = True
			continue

		hash_md5 = hashlib.md5()

		with open(filepath, "rb") as f:
			for chunk in iter(lambda: f.read(4096), b""):
				hash_md5.update(chunk)

		if hash_md5.hexdigest() != validhash:
			hashMismatch = True


	print( "\r ", end="" )

	if hashMismatch:
		print( "[" + ANSI_COLOR_RED + "hash" + ANSI_COLOR_END + "] ", end="" )
	else:
		print( "[" + ANSI_COLOR_GREEN + "hash" + ANSI_COLOR_END + "] ", end="" )


	if BIN_VALGRIND != "":
		if valgrindError:
			print( "[" + ANSI_COLOR_RED + "leak" + ANSI_COLOR_END + "] ", end="" )
		else:
			print( "[" + ANSI_COLOR_GREEN + "leak" + ANSI_COLOR_END + "] ", end="" )


	print( ANSI_COLOR_CYAN + aafFileName + ANSI_COLOR_END )


	if valgrindError or hashMismatch:
		print( "   :: Test command : " + testCmd )
		errorCounts+=1

	if valgrindError:
		print( "   :: valgrind log : " + valgrindOutputFile )

	# if errorCounts and args.run_from_cmake:
	# 	sys.exit(errorCounts);



def update( aafFileName, aaftoolAddCmd ):

	print( " [....] " + ANSI_COLOR_ORANGE + aafFileName + ANSI_COLOR_END, end="" )

	aafFile = TEST_AAF_DIR + DIR_SEP + aafFileName;
	expectedAaftoolOutputFile = TEST_EXPECTED_DIR + DIR_SEP + aafFileName + ".expected"

	if not os.path.isfile(aafFile):
		print( " : " + ANSI_COLOR_RED + "Missing AAF file \"" + aafFile + "\"" + ANSI_COLOR_END )
		return

	testCmd = AAFTOOL_CMD + " " + aaftoolAddCmd + " \"" + aafFile + "\" --verb 0 --no-color --relative-path --log-file \"" + expectedAaftoolOutputFile + "\""

	proc = subprocess.run( testCmd, capture_output=True, shell=True, text=True )

	print( "\r [" + ANSI_COLOR_GREEN + " ok " + ANSI_COLOR_END + "] " + ANSI_COLOR_ORANGE + aafFileName + ANSI_COLOR_END )



# sys.exit(errorCounts);





print("")

test("MC_Empty.aaf",                               "")
test("DR_Empty.aaf",                               "")

test("MC_Markers.aaf",                             "")
test("DR_Markers.aaf",                             "")
test("MC_Metadata.aaf",                            "")

test("MC_Clip_Mute.aaf",                           "")
test("MC_Track_Solo_Mute.aaf",                     "")

test("MC_Fades.aaf",                               "")
test("PR_Fades.aaf",                               "")
test("PT_Fades.aaf",                               "")
test("PT_Fades_A.aaf",                             "--pt-remove-sae")
test("PT_Fades_B.aaf",                             "--pt-true-fades")
test("PT_Fades_no_handles.aaf",                    "")

test("PT_Audio_Levels-noEMCC-noExpTracksAsMultiChannel.aaf", "")
test("MC_Audio_Levels.aaf",                        "") # verify AAFUsage_SubClip and AAFUsage_AdjustedClip
test("PR_Audio_Levels-noBTM.aaf",                  "")
test("DR_Audio_Levels.aaf",                        "")

test("PR_Audio_Pan-noBTM.aaf",                     "")
test("MC_Audio_Pan.aaf",                           "")

test("MC_Audio_Warp.aaf",                          "")
test("PR_Clip_length_beyond_EOF.aaf",              "")

test("PT_WAV_External.aaf",                        "--media-location \"" + TEST_MEDIA_DIR + "\"")
test("PT_WAV_External_sub_directory.aaf",          "--samplerate 44100")
test("PT_WAV_External_same_directory.aaf",         "--samplerate 44100")

test("PR_WAV_Internal.aaf",                        "")
test("PT_AIFF_External.aaf",                       "")
test("PR_AIFF_Internal.aaf",                       "")
test("PT_MXF_External.aaf",                        "")
test("PT_PCM_Internal.aaf",                        "--samplerate 44100")
test("DR_MP3_External.aaf",                        "")
test("PT_UTF8_EssencePath.aaf",                    "")

test("DR_Mono_Clip_Positioning.aaf",               "")
test("DR_Stereo_Clip_Positioning.aaf",             "--samplerate 48000")

test("DR_Multichannel_stereo_single_source.aaf",   "--samplerate 44100")
test("PT_Multichannel_stereo_multi_source.aaf",    "--pt-remove-sae")
test("DR_Multichannel_5.1_single_source.aaf",      "")
test("PT_Multichannel_5.1_multi_source.aaf",       "--pt-remove-sae")
test("DR_Multichannel_7.1_single_source.aaf",      "")
test("PT_Multichannel_7.1_multi_source.aaf",       "--pt-remove-sae")

test("MC_TC_23.976.aaf",         "--pos-format tc")
test("MC_TC_24.aaf",             "--pos-format tc")
test("MC_TC_25.aaf",             "--pos-format tc")
test("MC_TC_29.97_NDF.aaf",      "--pos-format tc")
test("MC_TC_29.97_DF.aaf",       "--pos-format tc")
test("MC_TC_30_NDF.aaf",         "--pos-format tc")
test("MC_TC_30_DF.aaf",          "--pos-format tc")
test("MC_TC_50.aaf",             "--pos-format tc")
test("MC_TC_59.94_NDF.aaf",      "--pos-format tc")
test("MC_TC_59.94_DF.aaf",       "--pos-format tc")
test("MC_TC_60_NDF.aaf",         "--pos-format tc")
test("MC_TC_60_DF.aaf",          "--pos-format tc")
test("MC_TC_100.aaf",            "--pos-format tc")
test("MC_TC_119.88_NDF.aaf",     "--pos-format tc")
test("MC_TC_119.88_DF.aaf",      "--pos-format tc")
test("MC_TC_120_NDF.aaf",        "--pos-format tc")
test("MC_TC_120_DF.aaf",         "--pos-format tc")

test("PT_lang_de.aaf",           "--pt-true-fades")
test("PT_lang_en.aaf",           "--pt-true-fades")
test("PT_lang_es.aaf",           "--pt-true-fades")
test("PT_lang_fr.aaf",           "--pt-true-fades")
test("PT_lang_ja.aaf",           "--pt-true-fades")
test("PT_lang_ko.aaf",           "--pt-true-fades")
test("PT_lang_zh_TW.aaf",        "--pt-true-fades")
test("PT_lang_zh_CN.aaf",        "--pt-true-fades")

print("")

extract("PR_WAV_Internal.aaf",  "--extract-clips --extract-format wav", [
	[ "3089dbaa3d9e03b820695504d5334d7c", "1_1_1000hz-18dbs16b44.1k.wav" ]
])
extract("PR_WAV_Internal.aaf",  "--extract-essences", [
	[ "b49538965723bb1840e01b6710da20b8", "1000hz-18dbs16b44.1k.wav" ]
])

extract("PR_AIFF_Internal.aaf", "--extract-clips --extract-format wav", [
	[ "44b9acf682cb12fa9c692f4e3c591079", "1_1_1000hz-18dbs16b44.1k.wav" ]
])
extract("PR_AIFF_Internal.aaf", "--extract-essences", [
	[ "694634f1af77e1c23e76b0b41d2b223f", "1000hz-18dbs16b44.1k.wav.aif" ]
])

extract("PT_PCM_Internal.aaf",  "--extract-clips --extract-format wav", [
	[ "2a8f46cf946e44973a4a73f84504a4c5", "1_1_1000hz-18dbs16b44.1k-01.wav" ]
])
extract("PT_PCM_Internal.aaf",  "--extract-essences --extract-format wav", [
	[ "2a8f46cf946e44973a4a73f84504a4c5", "1000hz-18dbs16b44.1k-01.wav" ]
])

print("")

sys.exit(errorCounts)
