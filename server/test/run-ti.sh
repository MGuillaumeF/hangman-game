cd ../bin
./HangmanGame 0.0.0.0 8080 . 1 &
sleep 30

echo "Integration tests START"

echo "**** Token POST Test START"

curl -X POST http://localhost:8080/api/token -H 'Content-Type: application/json' -H 'Accept: application/json' -d '{"login" : "a", "password" : "b"}'
sleep 30

echo "**** Token POST Test END"

echo "Soft stop emitted"
pkill -1 HangmanGame
echo "Integration tests END"
