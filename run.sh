#!/bin/bash

# ./run.sh starts the main binary. 
# It can be easily configured running ./run.sh configure
# and then, to start the main binary, just run ./run.sh.

LOW='\033[0;33m'
NC='\033[0m'

quit()
{
    echo; echo; exit $1
}

run_cmd()
{
    printf "${LOW}${1}${NC}\n"
    eval $1
}

# Here we go...

echo; echo

if [[ $1 =~ "configure" ]]
    then
    # Configure

    cat <<END 
    Please provide the IP address of the computer currently 
    running OSCulator, the Arduino's serial Baud Rate, and 
    the Arduinos' device names, in the form [path]:[n_vars], 
    where [path] is the full path to the Arduino serial device 
    (e.g. '/dev/cu.usbmodem1411', '/dev/ttyACM0'), and [n_vars] 
    is the number of variables each one of them is sending 
    (e.g., 4, 6, 12).
END
    echo

    echo -n "[ip address]: "; read param_ip
    echo -n "[baud rate]: "; read param_baud
    echo -n "[dev names]: "; read param_devices

    echo $param_ip > .config
    echo $param_baud >> .config
    echo $param_devices >> .config

    echo
    echo "Configuration file created."

elif [[ $1 =~ "build" ]]
    then
    # Build executables

    if [[ $2 =~ "tests" ]]
        then
        echo "Compiling test executables."
        cmd="make clean tests"
        run_cmd "${cmd}"
    else
        echo "Compiling executable for live concert."
        cmd="make clean prod"
        run_cmd "${cmd}"
    fi

elif (( $# == 0 ))
    then
    # Run main app

    if [[ ! -f .config ]]
        then
        echo "Configuration file not found."
        ./run.sh configure
    fi

    echo "Reading configuration file."
    read -d '\n' <.config param_ip param_baud param_devices
    cmd="bin/oscLiveMain $param_ip $param_baud $param_devices"
    echo

    for l in $param_devices
    do
        devname=`echo $l | cut -f1 -d':'`
	if [[ ! -c $devname ]]
	    then
	    echo "Device '$devname' not available."
	    quit 1
	fi
        inputs=`echo $l | cut -f2 -d':'`
        echo "Checking device '$devname' availability."
        cmd_nest="python3 check_device.py $devname $param_baud $inputs"
	trials=0
	while (( $trials < 10 ))
	do
            run_cmd "$cmd_nest"
	    retval=$?
	    if (( $retval == 0 ))
            then
		echo "Device '$devname' ready."
		break
            else
		trials=$(( $trials + 1 ))
		if (( $trials >= 10 ))
		then
		    echo "Device '$devname' not available."
		    quit 1
		fi
            fi
	done
    done

    echo
    echo "All devices ready."

    if [[ ! -f bin/oscLiveMain ]]
        then
        echo
        echo "No executable found. Building."
        ./run.sh build
    fi

    echo "Starting Arduino-osculator bridge..."
    run_cmd "$cmd"

else
    echo "usage: ./run.sh [configure|build tests]"
    quit 1

fi

echo; echo

