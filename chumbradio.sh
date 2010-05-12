#!/bin/sh
# $Id$
# /etc/init.d script to start/stop/load/unload/restart

MYSVC=chumbradio

# Support debug flag
if [ "$1" = "-x" ]
then
        set $1
        shift
fi

ACTION=$1
if [ "${ACTION}" = "" ]
then
	echo "Usage: $0 {start|stop|restart|status} [arg-list-file]"
	echo "  or : $0 {load|unload} devicepath [arg-list-file]"
	exit 1
fi
shift

if [ "${ACTION}" = "load" -o "${ACTION}" = "unload" ]
then
        DEVPATH=$1
        shift
fi

# List of arguments, quoted - may be empty
ARG_LIST_FILE=$1

. script_locations.sh
. ${ETC_INIT_DIR}/daemon-utils.sh


case ${ACTION} in
	start) loadService ${MYSVC} ${ARG_LIST_FILE} ;;
	stop) unloadService ${MYSVC} ;;
	restart) $0 stop ; $0 start $@ ;;
	status) xmlStatus ${MYSVC} ;;
	load|unload)
		if [ "${ACTION}" = "load" ]
		then
			loadForDevice ${DEVPATH} ${MYSVC} ${ARG_LIST_FILE}
		else
			unloadDeviceProcess ${DEVPATH} ${MYSVC}
		fi
		;;
	*)
		echo "$0: unknown action ${ACTION} - valid actions are start, stop, restart, status, load, unload"
		exit 1
		;;
esac

return 0
