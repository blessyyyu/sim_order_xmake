id=`ps aux|grep redis|grep -v grep|awk '{print $2}'`
echo $id
kill -9 $id
