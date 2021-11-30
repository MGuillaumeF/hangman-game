cd ../bin
./HangmanGame 0.0.0.0 8080 . 1 2>&1 > ../server/reports/integration-tests-report.log &
sleep 15

echo "Integration tests START"

./HangmanGameITTest
sleep 60

echo "Soft stop emitted"
pkill -15 HangmanGame
echo "Integration tests END"
