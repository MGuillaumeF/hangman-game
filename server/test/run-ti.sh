cd ../bin
./HangmanGame 0.0.0.0 8080 . 1 &
sleep 30

echo "\n Integration tests START"

echo "\n **** Token POST Test START"

curl -X POST http://localhost:8080/api/token -H 'Content-Type: application/json' -H 'Accept: application/json' -d '{"login" : "a", "password" : "b"}'
sleep 30

echo "\n **** Token POST Test END"

echo "\n Soft stop emitted"
pkill -15 HangmanGame && echo "\n SHUTDOWN terminated with SUCCESS" || echo "\n SHUTDOWN Failed"
echo "\n Integration tests END"
