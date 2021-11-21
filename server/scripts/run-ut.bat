@echo off
cd %~dp0../bin
call cmd.exe /C "HangmanGameTest.exe --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=%~dp0../reports/reports.xml  --report_sink=%~dp0../reports/reportUT.xml"
cd %~dp0