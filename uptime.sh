i=0
sleep 1
while [ "$i" != 10 ]
do
	uptime
	sleep 1
	i=$(($i+1))
done
exit 0
