@echo off
copy Vorbis_Dynamic_Debug/vorbis_d.dll  C:\Users\sabawi\programs\MyPrograms\narveso2\Debug
if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: A tool returned an error code from "Performing Post-Build Event..."
exit 1
:VCEnd