DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))
DIR_SRC=$DIR_TOP/src
DIR_BUILD=$DIR_TOP/build

function cmake-this {
    if [ -e $DIR_BUILD ] ; then
	echo "Clean up the build directory..."
	\rm -rf $DIR_BUILD
    fi
    mkdir -p $DIR_BUILD
    echo "Run cmake..."
    ( cd $DIR_BUILD && cmake $DIR_SRC )
    local RET=$?
    if [ $RET -eq 0 ] ; then
	echo "OK.  Run 'make-this' at any time/directory."
    else
	echo "NG.  Need a fix."
    fi
    return $RET
}

function make-this {
    if [ ! -e $DIR_BUILD/Makefile ] ; then
	echo "The build directory is not ready.  Complete 'cmake-this' first."
	return 1
    fi
    ( cd $DIR_BUILD && make )
    return $?
}

function install-this {
    if [ ! -e /etc/systemd/system/mcc-hat-server.service ] ; then
	echo "Installing 'mcc-hat-server.service'..."
	sudo cp -a $DIR_TOP/etc/mcc-hat-server.service /etc/systemd/system
	sudo systemctl daemon-reload
    fi

    local -r ACTIVE=$(sudo systemctl is-active mcc-hat-server)
    if [ $ACTIVE = 'active' ] ; then
	echo "Stopping the server via systemd..."
	sudo systemctl stop mcc-hat-server
    fi
    echo "Copying 'mcc-hat-server' to '/usr/local/bin'..."
    sudo cp -a $DIR_TOP/build/mcc-hat-server /usr/local/bin
    if [ $ACTIVE = 'active' ] ; then
	echo "Starting the server via systemd..."
	sudo systemctl start mcc-hat-server
    else
	echo "The server is not running.  You might use the following commands."
	echo "  sudo systemctl enable mcc-hat-server"
	echo "  sudo systemctl start  mcc-hat-server"
	echo "  sudo systemctl status mcc-hat-server"
    fi
}
