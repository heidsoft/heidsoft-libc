#include <iostream>
#include <string>
#include <vector>
#include <map>  // 添加map头文件
#include <snappy.h>

// 前置声明示例结构体和类
struct Metric {
    std::string name;
    double value;
    // 可根据实际需求添加更多字段
};

class Database {
public:
    std::string exportData() const {  // 修改函数名，避免使用关键字
        return "database_export_data";
    }
};

int main() {
    // 测试数据 - 改用更大的文本
    std::string input = R"(This is a much longer test string to demonstrate Snappy compression. 
    Snappy is a compression/decompression library that prioritizes speed over compression ratio.
    It does not aim for maximum compression, but aims for very high speeds and reasonable compression...)";
    std::string compressed;
    std::string decompressed;
    
    // 1. 压缩数据
    snappy::Compress(input.data(), input.size(), &compressed);
    std::cout << "原始大小: " << input.size() 
              << " bytes, 压缩后: " << compressed.size() 
              << " bytes (压缩率: " 
              << (100.0 * compressed.size() / input.size()) << "%)\n";
    
    // 2. 解压数据
    bool success = snappy::Uncompress(compressed.data(), compressed.size(), &decompressed);
    if (!success) {
        std::cerr << "解压失败!\n";
        return 1;
    }
    
    // 3. 验证结果
    if (input != decompressed) {
        std::cerr << "解压数据不匹配!\n";
        return 1;
    }
    
    std::cout << "测试成功 - 原始数据与解压数据完全匹配\n";
    return 0;
}

// 日志压缩存储示例
void compressLog(const std::string& rawLog) {
    std::string compressed;
    snappy::Compress(rawLog.data(), rawLog.size(), &compressed);
    // 写入压缩后的日志文件
}

// 修改后的工具函数：

// 监控指标压缩传输
std::string compressMetrics(const std::vector<Metric>& metrics) {
    std::string serialized;
    for (const auto& m : metrics) {
        serialized += m.name + ":" + std::to_string(m.value) + "\n";
    }
    std::string compressed;
    snappy::Compress(serialized.data(), serialized.size(), &compressed);
    return compressed;
}

// 批量配置文件压缩
// 修改批量配置文件压缩函数，避免使用C++17结构化绑定
void distributeConfigs(const std::map<std::string, std::string>& configs) {
    std::string serialized;
    for (const auto& config_pair : configs) {
        serialized += config_pair.first + "=" + config_pair.second + "\n";
    }
    std::string compressed;
    snappy::Compress(serialized.data(), serialized.size(), &compressed);
    // 分发压缩后的配置
}

// 修改数据库备份压缩函数，使用新的函数名
void backupDatabase(const Database& db) {
    std::string dump = db.exportData();  // 使用修改后的函数名
    std::string compressed;
    snappy::Compress(dump.data(), dump.size(), &compressed);
    // 存储压缩备份
}

// 容器层数据压缩
std::string compressDockerLayer(const std::string& layerData) {
    std::string compressed;
    snappy::Compress(layerData.data(), layerData.size(), &compressed);
    return compressed;
}