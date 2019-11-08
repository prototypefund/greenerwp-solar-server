#!/bin/sh

##
# Calls IP update service if server is not reachable.
#
# Set DEBUG environment variable to see some debug messages.

# The IP to be set
MY_IP=
MY_IPV6=

# URL to check
SERVER_URL="https://www.example.com"

# Seconds to wait for server response
TIMEOUT=30

IP_UPDATE_URL="https://www.example.com/update?myip=$MY_IP&myipv6=$MY_IPV6"
IP_UPDATE_AUTH='username:password'

CURL="curl"

[ $DEBUG ] && echo Checking server >&2
$CURL "$SERVER_URL" --head --silent --max-time $TIMEOUT > /dev/null
if [ $? -ne 0 ]; then
    [ $DEBUG ] && echo Check failed, updating IP >&2
    $CURL --silent --user "$IP_UPDATE_AUTH" "$IP_UPDATE_URL" > /dev/null
    [ $? -ne 0 ] && echo IP update failed >&2 && exit 1
fi;
