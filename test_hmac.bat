@echo off
REM Git-Lite HMAC Authentication Demo
REM Windows Batch Script

echo ======================================
echo Git-Lite HMAC-SHA256 Demo
echo ======================================
echo.

REM Step 1: Clean previous test
echo [1] Cleaning previous test...
if exist TestProject rmdir /s /q TestProject
if exist .gitlite rmdir /s /q .gitlite
echo.

REM Step 2: Initialize repository
echo [2] Initializing repository...
vcs init TestProject
cd TestProject
echo.

REM Step 3: Create first commit
echo [3] Creating first commit...
echo Hello World > file1.txt
vcs add file1.txt
vcs commit -m "Initial commit"
echo.

REM Step 4: Create second commit
echo [4] Creating second commit...
echo Second file > file2.txt
vcs add file2.txt
vcs commit -m "Added file2"
echo.

REM Step 5: Show log (all valid)
echo [5] Showing commit log (all valid)...
vcs log
echo.

REM Step 6: Tamper with commit
echo [6] TAMPERING: Modifying commit message...
for /f %%i in ('dir /b /o-d .gitlite\commits\*.txt ^| findstr /n "^" ^| findstr "^1:"') do set COMMIT_FILE=%%i
set COMMIT_FILE=%COMMIT_FILE:~2%

echo Tampering with: %COMMIT_FILE%

REM Backup original
copy ".gitlite\commits\%COMMIT_FILE%" ".gitlite\commits\%COMMIT_FILE%.backup" > nul

REM Modify message (PowerShell for text replacement)
powershell -Command "(Get-Content '.gitlite\commits\%COMMIT_FILE%') -replace 'Added file2', 'HACKED MESSAGE' | Set-Content '.gitlite\commits\%COMMIT_FILE%'"

echo Changed message to 'HACKED MESSAGE'
echo.

REM Step 7: Show log after tampering
echo [7] Showing log after tampering...
vcs log
echo.

REM Step 8: Restore
echo [8] Restoring original commit...
copy ".gitlite\commits\%COMMIT_FILE%.backup" ".gitlite\commits\%COMMIT_FILE%" > nul
del ".gitlite\commits\%COMMIT_FILE%.backup"
echo.

REM Step 9: Verify restoration
echo [9] Final verification...
vcs log
echo.

echo ======================================
echo Demo Complete!
echo ======================================
echo.
echo What was demonstrated:
echo   - Commit signing with HMAC-SHA256
echo   - Signature verification
echo   - Tamper detection
echo   - Integrity validation

cd ..
pause