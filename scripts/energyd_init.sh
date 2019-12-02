#! /bin/sh

### BEGIN INIT INFO
# Provides:		energyd
# Default-Start:	2 3 4 5
# Default-Stop:	
# Short-Description:	LowTechWP Energy Daemon
### END INIT INFO

set -e

# /etc/init.d/ssh: start and stop the OpenBSD "secure shell(tm)" daemon

test -x /usr/bin/energyd || exit 0

umask 022

if test -f /etc/default/energyd; then
	. /etc/default/energyd
fi

. /lib/lsb/init-functions

if [ -n "$2" ]; then
	ENERGYD_OPTS="$ENERGYD_OPTS $2"
fi

# Are we running from init?
run_by_init() {
	([ "$previous" ] && [ "$runlevel" ]) || [ "$runlevel" = S ]
}


check_dev_null() {
	if [ ! -c /dev/null ]; then
		if [ "$1" = log_end_msg ]; then
			log_end_msg 1 || true
		fi
		if ! run_by_init; then
			log_action_msg "/dev/null is not a character device!" || true
		fi
		exit 1
	fi
}

export PATH="${PATH:+$PATH:}/usr/sbin:/sbin"

case "$1" in
	start)
		check_dev_null
		log_daemon_msg "Starting LowTechWP Energy Daemon" "energyd" || true
		if start-stop-daemon --background --start --make-pidfile --quiet --oknodo --chuid 0:0 --pidfile /run/energyd.pid --exec /usr/bin/energyd -- daemon $ENERGYD_OPTS; then
			log_end_msg 0 || true
		else
			log_end_msg 1 || true
		fi
		;;
	stop)
		log_daemon_msg "Stopping LowTechWP Energy Daemon" "energyd" || true
		if start-stop-daemon --stop --remove-pidfile --quiet --oknodo --pidfile /run/energyd.pid --exec /usr/bin/energyd; then
			log_end_msg 0 || true
		else
			log_end_msg 1 || true
		fi
		;;

	restart)
		log_daemon_msg "Restarting LowTechWP Energy Daemon" "energyd" || true
		start-stop-daemon --stop --remove-pidfile --quiet --oknodo --retry 30 --pidfile /run/energyd.pid --exec /usr/bin/energyd
		check_for_no_start log_end_msg
		check_dev_null log_end_msg
		if start-stop-daemon --background --make-pidfile --start --quiet --oknodo --chuid 0:0 --pidfile /run/energyd.pid --exec /usr/bin/energyd -- daemon $ENERGYD_OPTS; then
			log_end_msg 0 || true
		else
			log_end_msg 1 || true
		fi
		;;

	status)
		status_of_proc -p /run/energyd.pid /usr/bin/energyd energyd && exit 0 || exit $?
		;;

	*)
		log_action_msg "Usage: /etc/init.d/energyd {start|stop|restart|status}" || true
		exit 1
esac

exit 0
