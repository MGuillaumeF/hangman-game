@echo off
rem create certificate authority
openssl req -x509 -sha256 -days 356 -nodes -newkey rsa:2048 -subj "/CN=hangman-game.mguillaumef.com/C=FR/L=France" -keyout rootCA.key -out rootCA.crt
rem ---- create self-signed certificates using openssl
rem create the server private key
openssl genrsa -out server.key 2048
rem generate certificate signing request (csr) using server private key
openssl req -new -key server.key -out server.csr -config csr.conf
rem generate ssl certificate with self signed ca
openssl x509 -req  -in server.csr -CA rootCA.crt -CAkey rootCA.key  -CAcreateserial -out server.crt -days 365 -sha256 -extfile cert.conf