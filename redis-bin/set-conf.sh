dir=`pwd`
mkdir -p /etc/redis
cp -rf $dir/redis-6379.conf  $dir/redis-6380.conf  $dir/redis-6381.conf  $dir/sentinel-26379.conf  $dir/sentinel-26380.conf  $dir/sentinel-26381.conf /etc/redis/
