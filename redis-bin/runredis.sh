pkill -9 redis
if [ ! -d '/var/lib/redis/6379' ];then
mkdir -p /var/lib/redis/6379
fi
if [ ! -d '/var/lib/redis/6380' ];then
mkdir -p /var/lib/redis/6380
fi
if [ ! -d '/var/lib/redis/6381' ];then
mkdir -p /var/lib/redis/6381
fi
echo "start redis-server begin"
/usr/local/bin/redis-server /etc/redis/redis-6379.conf
sleep 1s
/usr/local/bin/redis-server /etc/redis/redis-6380.conf
sleep 1s
/usr/local/bin/redis-server /etc/redis/redis-6381.conf
echo "start redis-server end"
sleep 1s
echo "start redis-sentinel begin"
/usr/local/bin/redis-sentinel /etc/redis/sentinel-26379.conf
sleep 1s
/usr/local/bin/redis-sentinel /etc/redis/sentinel-26380.conf
sleep 1s
/usr/local/bin/redis-sentinel /etc/redis/sentinel-26381.conf
echo "start redis-sentinel end"
ps -ef|grep redis
