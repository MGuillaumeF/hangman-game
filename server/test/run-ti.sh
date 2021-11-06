cd ../bin
./HangmanGame 0.0.0.0 8080 . 1 &
sleep 30
curl -X POST http://localhost:8080/api/token -H 'Content-Type: application/json' -H 'Accept: application/json' -d '{"login" : "a", "password" : "b"}'
sleep 30
echo "soft stop emitted"
pkill -15 HangmanGame
