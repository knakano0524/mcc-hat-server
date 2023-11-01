# mcc-hat-server

Program to control MCC HAT via the TCP/IP socket communication.


## Installation

```
source setup.sh
cmake-this
make-this
sudo cp -a inst/bin/mcc-hat-server /usr/local/bin
```


## Registration to systemd

```
sudo cp sudo cp -a etc/mcc-hat-server.service /etc/systemd/system
sudo systemctl daemon-reload
sudo systemctl enable mcc-hat-server
sudo systemctl start  mcc-hat-server
sudo systemctl status mcc-hat-server
```


## Read-Only Mode of Raspberry Pi

To be written.


## Reference

- [MCC DAQ HAT Library documentation](https://mccdaq.github.io/daqhats/)

## Contact

Kenichi Naknao <knakano0524@gmail.com>
