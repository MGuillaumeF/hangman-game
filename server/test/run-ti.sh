cd ../bin
./HangmanGame 0.0.0.0 8080 . 1 &
sleep 30
curl http://localhost:8080/api/token
sleep 15
pkill -15 a.out