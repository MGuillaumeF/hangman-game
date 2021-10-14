@echo off
java -jar swagger-codegen-cli-2.2.1.jar generate -i %~dp0..\..\server\docs\api_doc.json -l html -o %~dp0docs
REM java -jar swagger-codegen-cli-3.0.0-rc1.jar generate -i %~dp0..\..\server\docs\api_doc.json -l html -o %~dp0docs
cd %~dp0docs
REM html html2 cwiki cpprest
REM call cmd.exe /C "npm install"
REM call cmd.exe /C "npm audit fix"
REM echo. Server listen at http://localhost:8002/
REM call cmd.exe /C "node main.js"
cd %~dp0