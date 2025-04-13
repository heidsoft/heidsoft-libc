# Snappy 压缩库学习与实践

## 编译说明

### 基本编译命令

```bash
clang++ -std=c++11 -O2 -o snappy_demo snappy_demo.cc \
-I/usr/local/include -L/usr/local/lib -lsnappy
```

## 运维产品中的典型应用

### 1. 日志系统优化

```cpp
// 日志压缩存储实现
void LogManager::compressAndStore(const std::string& logEntry) {
    std::string compressed;
    snappy::Compress(logEntry.data(), logEntry.size(), &compressed);
    storage_.write(compressed);  // 写入压缩后的日志
}
```
