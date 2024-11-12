@ECHO OFF

SET INCLUDE=%~DP0INCLUDE
SET LIB=%~DP0LIB

FOR /F "TOKENS=1* DELIMS= " %%I IN ("%*") DO "%~DP0BIN\%%I" %%J /link gdi32.lib kernel32.lib user32.lib ole32.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib
