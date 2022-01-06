cd ../bin
./HangmanGame 2>&1 > ./integration-tests-report.log &
sleep 15

echo "tests START"

ctest --verbose || (cat ../reports/logUT.xml && cat ../reports/logIT.xml && cat Testing/Temporary/LastTest.log && exit 1)

sleep 60

echo "Soft stop emitted"
pkill -15 HangmanGame
echo "Integration tests END"
