cd ../bin
./HangmanGame 0.0.0.0 8080 . 1 &
sleep 15

echo "Integration tests START"

./HangmanGameITTest
sleep 20

echo "Soft stop emitted"
pkill -15 HangmanGame
echo "Integration tests END"
