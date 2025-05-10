## sim_order xmake version
依赖的三方库hiredis和redis-plus-plus 放在/public/third_party 下


提前安装好哨兵模式的redis：
cd redis-bin
./install_bin.sh
./set_conf.sh

### 启动redis
./runredis.sh


### 停止redis
./killredis.sh

项目根目录下: xmake build;  xmake run即可