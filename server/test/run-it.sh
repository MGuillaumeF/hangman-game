cd ../bin
./HangmanGame 0.0.0.0 8080 1 2>&1 > ./integration-tests-report.log &
sleep 15

echo "Integration tests START"

./HangmanGameITTest --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=../reports/logIT.xml  --report_sink=../reports/reportIT.xml
sleep 60

echo "Soft stop emitted"
pkill -15 HangmanGame
echo "Integration tests END"
