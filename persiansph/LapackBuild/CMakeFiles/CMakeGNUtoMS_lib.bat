@echo off
set VSCMD_START_DIR=.
call "F:\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
lib /machine:"x86" %*
