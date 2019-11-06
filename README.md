# simhash
c++ simhash 排重算法库

参考
http://yanyiwu.com/work/2014/01/30/simhash-shi-xian-xiang-jie.html
大牛的文章，由于其仓库的simhash使用的库不更新了，所有自己按照作者思想写了一个，使用submodule随时更新最新版本
不同的hash算法和关键词提取密度会对结果产生很大的影响
项目里集成了smhasher，可以随时切换hash算法
请使用者自行选择合适的hash算法和设置关键词密度

我测试的最适中的hash算法是cityhash

使用方法：

git clone xxx

git submodule init

git submodule update

cd xxx
mkdir build
cd build
cmake ..
make -j4

cd bin

./simhash
