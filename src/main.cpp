#include <cstdio>
#include <iostream>
#include <string.h>
#include <vector>
#include <utility>

#include "GlogHelper.h"
#include "cppjieba/KeywordExtractor.hpp"

// #include "Spooky.h"
// #include "MurmurHash2.h"
#include "City.h"
// #include "farmhash.h"
// #include "xxh3.h"


class Simhasher
{
    public:
        const int BITS_LENGTH = 64;
        const uint64_t HASH_SEED = 7789;
        cppjieba::KeywordExtractor extractor;

        Simhasher(const std::string& dictPath, const std::string& modelPath, const std::string& idfPath, const std::string& stopWords):
            // 初始化 extractor
            extractor(dictPath, modelPath, idfPath, stopWords)
        {}

        uint64_t calculation(const std::string& content)
        {
            // 关键词、权重集合
            std::vector<std::pair<std::string, double>> keywordWeights;
            // hash值、权重集合
            std::vector<std::pair<uint64_t, double>> tmp;

            // 100个字抽20个关键词 content.length() / 100 * 20
            size_t topN = content.length() / 100 * 1000;
            LOG(INFO) << "内容字数：" << content.length() / 3;
            LOG(INFO) << "设置关键词提取数量：" << topN;

            // 抽取关键词
            extractor.Extract(content, keywordWeights, topN);
            LOG(INFO) << "实际关键词提取数量：" << keywordWeights.size();
            LOG(INFO) << "关键词集合：" << keywordWeights;

            // 为tmp分配空间
            tmp.resize(keywordWeights.size());
            // 为每个关键词，计算hash值
            // SpookyHash spookyHash;
            for(size_t i = 0; i < tmp.size(); i++)
            {
                // tmp[i].first = spookyHash.Hash64(keywordWeights[i].first.c_str(), keywordWeights[i].first.size(), HASH_SEED);
                // tmp[i].first = MurmurHash64A(keywordWeights[i].first.c_str(), keywordWeights[i].first.size(), HASH_SEED);
                tmp[i].first = CityHash64(keywordWeights[i].first.c_str(), keywordWeights[i].first.size());
                // tmp[i].first = util::Hash64(keywordWeights[i].first.c_str(), keywordWeights[i].first.size());
                // tmp[i].first = XXH3_64bits(keywordWeights[i].first.c_str(), keywordWeights[i].first.size());
                LOG(INFO) << "关键词 [ " << keywordWeights[i].first << " ] HASH 值为：" << tmp[i].first;
                tmp[i].second = keywordWeights[i].second;
            }

            // 根据keywordWeights集合，按二进制位，纵向累加的权重值集合
            std::vector<double> weights(BITS_LENGTH, 0.0);
            // 二进制位游标
            const uint64_t binaryCursor = 1;
            // 遍历hash值和权重值集合，对权重进行纵向累加，
            for(size_t i = 0; i < tmp.size(); i++)
            {
                // 遍历二进制位纵向累加权重
                for(size_t j = 0; j < BITS_LENGTH; j++)
                {
                    // 判断二进制位，如果该位是1，则 + 权重，如果是0，则 - 权重
                    weights [j] += (((binaryCursor << j) & tmp[i].first) ? 1: -1) * tmp[i].second;
                }
            }

            // 汉明距离基数
            uint64_t hammingBase = 0;
            // 遍历weights集合中的权重值，生成汉明距离基数
            for(size_t j = 0; j < BITS_LENGTH; j++)
            {
                // 正数为1,负数为0
                if(weights[j] > 0.0)
                {
                    hammingBase |= (binaryCursor << j);
                }
            }
            std::string hammingBaseStr;
            toBinaryString(hammingBase, hammingBaseStr);
            LOG(INFO) << "汉明距离基数为：" << hammingBaseStr;

            return hammingBase;
        }

        int hammingDistance(uint64_t base_1, uint64_t base_2)
        {
            unsigned short cnt = 0;
            // 异或运算
            base_1 ^= base_2;
            // 计算二进制1的个数
            int distance = __builtin_popcountll(base_1);
            LOG(INFO) << "汉明距离为：" << distance;
            return distance;
        }

        // 将整形转化为二进制形式字符串
        static void toBinaryString(uint64_t req, std::string& res)
        {
            res.resize(64);
            for(signed i = 63; i >= 0; i--)
            {
                req & 1 ? res[i] = '1' : res[i] = '0';
                req >>= 1;
            }
        }
};



int main()
{
    // 开启glog日志记录模块
    GlogHelper gloghelper("zhasher");
    LOG(INFO) << "I am INFO!";

    // uint64_t u64_1(1);
    // u64_1 = u64_1 << 1;
    // std::cout << "u64_1：" << u64_1 << std::endl;
    // u64_1 = u64_1 << 1;
    // std::cout << "u64_1：" << u64_1 << std::endl;
    // u64_1 = u64_1 << 1;
    // std::cout << "u64_1：" << u64_1 << std::endl;

    std::string s1 = "我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。";
    std::string s2 = "我是蓝翔技工拖拉机学院手扶拖拉机专业的。多久，我就会升职加薪，总经理，出任CEO，走上人生巅峰。";
    // size_t topN = s.length() / 100 * 20;
    // std::cout << "文章长度：" << s.length() << std::endl;
    // std::cout << "topN：" << topN << std::endl;
    // uint64 h = CityHash64(s.c_str(), s.length());
    // std::cout << h << std::endl;

    std::string dictPath= "/home/pray/zhj/project/c++/zhasher/external/cppjieba/dict/jieba.dict.utf8";
    std::string modelPath= "/home/pray/zhj/project/c++/zhasher/external/cppjieba/dict/hmm_model.utf8";
    std::string idfPath= "/home/pray/zhj/project/c++/zhasher/external/cppjieba/dict/idf.utf8";
    std::string stopWords= "/home/pray/zhj/project/c++/zhasher/external/cppjieba/dict/stop_words.utf8";

    // cppjieba::KeywordExtractor _extractor(dictPath, modelPath, idfPath, stopWords);
    // std::vector< std::pair<std::string,double> > res;
    // _extractor.Extract(s, res, topN);
    // std::cout << res << std::endl;

    Simhasher simhasher(dictPath, modelPath, idfPath, stopWords);
    uint64_t b1 = simhasher.calculation(s1);
    uint64_t b2 = simhasher.calculation(s2);
    int d = simhasher.hammingDistance(b1, b2);
    
}