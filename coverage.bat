
cd bin
cmd.exe /C "Application.exe"
cd ../obj
C:\Users\miche\Desktop\LLVM\bin\llvm-cov.exe gcov main.gcno -a -u -b -c > ../reports/summary.txt
@REM dans obj
@REM set LLVM_PROFILE_FILE=example.profraw
@rem cd ../bin
@rem C:\Users\miche\Desktop\LLVM\bin\llvm-cov show  --instr-profile PROF Application.exe ../src/main.cpp
@REM cd ../obj
@REM C:\Users\miche\Desktop\LLVM\bin\llvm-profdata merge -sparse main.profraw -o main.profdata

@REM cd ../bin

@REM C:\Users\miche\Desktop\LLVM\bin\llvm-cov show ./Application.exe -instr-profile=../obj/main.profdata

@rem C:\Users\miche\Desktop\LLVM\bin\llvm-cov.exe export main.gcno -format=lcov

