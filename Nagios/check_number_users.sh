#!/bin/sh
	old=`cat /var/lib/nagios/numberusersold | head -1`
	/usr/bin/wget -O /var/lib/nagios/numberusers http://wvm061.dei.isep.ipp.pt/databaseWS/Database.svc/numberusers
	#echo $(pwd) > /tmp/teste
	chmod 777 /var/lib/nagios/numberusers
	now=`cat /var/lib/nagios/numberusers | head -1`
	if [ $old -eq $now ] 
	then
		mv -f /var/lib/nagios/numberusers /var/lib/nagios/numberusersold
		chmod 777 /var/lib/nagios/numberusersold
		echo "OK"
		exit 0
	else
		mv -f /var/lib/nagios/numberusers /var/lib/nagios/numberusersold
		chmod 777 /var/lib/nagios/numberusersold
		echo "WARN "
		exit 1
	fi
	

