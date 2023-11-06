# mcc-hat-server

Program to control MCC HAT via the TCP/IP socket communication.


## Building & Testing

```
source setup.sh
cmake-this
make-this
./build/mcc-hat-server -ivv
```


## Installation & Registration to systemd

```
install-this
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
