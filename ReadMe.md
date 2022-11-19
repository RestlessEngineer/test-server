# Description

Simple server for trying and exploring **uWebSocket** technology.

## installations

**openssl** and **json** library

```
sudo apt install libssl-dev nlohmann-json3-dev
```

### build uWebSockets

load projects **uWebSockets**

```
git clone --recurse-submodules git@github.com:uNetworking/uWebSockets.git
cd uWebSockets
WITH_OPENSSL=1 make
```

Then you have to install **uWebSockets**

```
 make DESTDIR=$HOME/.local prefix= install
```

You also need to copy uSockets

```
cp -r uSockets $HOME/.local/
cp  uSockets/uSockets.a $HOME/.local/lib
```

Ensure that you have **lib** directory!

### final step

```
mkdir build
cd build
cmake ../
```

THAT'S ALL!

## some useful commands

for create connection 
```
 ws = new WebSocket("ws://127.0.0.1:9001/"); 
 ws.onmessage = ({data}) => console.log(">SERVER>", data);
 ```
for send message
```
ws.send('your message')
```