cd ../../bin/server/app
./HangmanGame 2>&1 > ./integration-tests-report.log &
sleep 15


cd ../../
echo "tests START"

ctest --verbose || (cat ../server/reports/logUT.xml && cat ../server/reports/logIT.xml && cat Testing/Temporary/LastTest.log && exit 1)

sleep 60

echo "Soft stop emitted"
pkill -15 HangmanGame
echo "Integration tests END"
