#!/bin/bash

# ./run.sh starts the main binary. 
# It can be easily configured running ./run.sh configure
# and then, to start the main binary, just run ./run.sh.

YLW='\033[0;33m'
NC='\033[0m'

IP_MATCH="(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})"

quit()
{
    echo; echo; exit $1
}

yellow_print()
{
    printf "${YLW}$@${NC}\n"
}

run_cmd()
{
    yellow_print "${1}"
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

    if [[ -f .config ]]
        then
        echo "Loading previous configuration."
        read -d '\n' <.config old_param_ip old_param_baud old_param_devices
        echo "Previous configuration was:"
        echo "  [ip address]: ${old_param_ip}"
        echo "  [baud rate]:  ${old_param_baud}"
        echo "  [dev names]:  ${old_param_devices}"
        echo "Press enter to skip input and keep a previous value."
        echo
    fi

    echo -n "[ip address]: "; read param_ip
    if [[ -z $param_ip ]]
        then
        if [[ -z $old_param_ip ]]
            then
            echo "Please provide an input."
            quit 1
        fi
        param_ip=$old_param_ip
    elif [[ ! $param_ip =~ ^(${IP_MATCH}\.){3}(${IP_MATCH})$ ]]
        then
        echo "Illegal ip address."
        quit 1
    fi

    echo -n "[baud rate]: "; read param_baud
    if [[ -z $param_baud ]]
        then
        if [[ -z $old_param_baud ]]
            then
            echo "Please provide an input."
            quit 1
        fi
        param_baud=$old_param_baud
    elif [[ ! $param_baud =~ ^[0-9]{1,}$ ]]
        then
        echo "Illegal baud rate."
        quit 1
    fi

    echo -n "[dev names]: "; read param_devices
    if [[ -z $param_devices ]]
        then
        if [[ -z $old_param_devices ]]
            then
            echo "Please provide an input."
            quit 1
        fi
        param_devices=$old_param_devices
    elif [[ ! $param_devices =~ ^(/dev/[^:]+:[0-9]{1,2} ?){1,}$ ]]
        then
        echo "Illegal device(s) name(s)."
        quit 1
    fi

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
        echo "Checking device '$devname' availability."
    	if [[ ! -c $devname ]]
    	    then
    	    echo "Device '$devname' not connected."
    	    quit 1
    	fi
        inputs=`echo $l | cut -f2 -d':'`
        cmd_nest="python3 check_device.py $devname $param_baud $inputs"
    	trials=0
        yellow_print $cmd_nest
    	while (( $trials < 10 ))
    	do
            eval "$cmd_nest"
    	    retval=$?
    	    if (( $retval == 0 ))
                then
        		echo "Device '${devname}' ready."
        		break
            else
        		trials=$(( $trials + 1 ))
                yellow_print "Attempt ${trials} failed."
        		if (( $trials >= 10 ))
        		then
        		    echo "Device '${devname}' not available."
                    echo "Maybe baud rate is not ${param_baud}?"
                    echo "Maybe '${devname}' is not sending ${inputs} variables?"
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

