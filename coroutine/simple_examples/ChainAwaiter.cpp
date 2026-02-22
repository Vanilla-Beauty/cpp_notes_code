// chain_awaiter_fixed.cpp
#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

// ==================== ChainAwaiter 定义 ====================
struct ChainAwaiter {
    std::coroutine_handle<> next_coroutine;
    const char* name;
    
    bool await_ready() const noexcept {
        return false;  // 总是需要挂起
    }
    
    // 返回 handle 实现协程链式调用
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> h) {
        std::cout << "  [" << name << "] 当前协程挂起，切换到下一个协程" << std::endl;
        return next_coroutine;  // 直接恢复下一个协程
    }
    
    void await_resume() noexcept {
        std::cout << "  [" << name << "] 协程恢复" << std::endl;
    }
};

// ==================== 协程返回类型定义 ====================
struct ChainTask {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    
    struct promise_type {
        // 存储待恢复的协程句柄（用于链式返回）
        std::coroutine_handle<> continuation{nullptr};
        
        ChainTask get_return_object() {
            return ChainTask(handle_type::from_promise(*this));
        }
        
        std::suspend_always initial_suspend() { 
            std::cout << "  [Promise] initial_suspend" << std::endl;
            return {}; 
        }
        
        // 关键修复：final_suspend 返回 suspend_always，让外部控制销毁
        std::suspend_always final_suspend() noexcept { 
            std::cout << "  [Promise] final_suspend" << std::endl;
            return {}; 
        }
        
        void return_void() {
            std::cout << "  [Promise] return_void" << std::endl;
        }
        
        void unhandled_exception() {
            std::cout << "  [Exception] 协程异常!" << std::endl;
        }
    };
    
    handle_type handle;
    
    ChainTask(handle_type h) : handle(h) {
        std::cout << "  [ChainTask] 构造" << std::endl;
    }
    
    ~ChainTask() {
        std::cout << "  [ChainTask] 析构" << std::endl;
        if (handle) handle.destroy();
    }
    
    ChainTask(ChainTask&& other) noexcept : handle(other.handle) {
        std::cout << "  [ChainTask] 移动构造" << std::endl;
        other.handle = nullptr;
    }
    
    ChainTask& operator=(ChainTask&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }
    
    // 启动协程链
    void start() {
        if (handle && handle.done() == false) {
            std::cout << "  [Main] 开始执行协程链..." << std::endl;
            handle.resume();
            
            // 协程链执行完后，手动清理所有协程
            cleanup();
        }
    }
    
    // 清理协程链（从后往前销毁）
    void cleanup() {
        std::cout << "  [Main] 清理协程链..." << std::endl;
        if (handle) {
            handle.destroy();
            handle = nullptr;
        }
    }
};

// ==================== 协程函数 ====================
ChainTask coroutine_a(ChainTask& next);
ChainTask coroutine_b(ChainTask& next);
ChainTask coroutine_c();

ChainTask coroutine_a(ChainTask& next) {
    std::cout << "  [Coroutine A] 开始执行" << std::endl;
    co_await ChainAwaiter{next.handle, "A"};
    std::cout << "  [Coroutine A] 恢复执行" << std::endl;
}

ChainTask coroutine_b(ChainTask& next) {
    std::cout << "  [Coroutine B] 开始执行" << std::endl;
    co_await ChainAwaiter{next.handle, "B"};
    std::cout << "  [Coroutine B] 恢复执行" << std::endl;
}

// 修复1: 添加 co_return 语句
ChainTask coroutine_c() {
    std::cout << "  [Coroutine C] 开始执行" << std::endl;
    std::cout << "  [Coroutine C] 这是最后一个协程" << std::endl;
    co_return;  // 显式返回
}

// ==================== 主函数 ====================
int main() {
    std::cout << "========== ChainAwaiter 示例 (修复版) ==========" << std::endl;
    std::cout << std::endl;
    
    // 修复2: 注意创建顺序，确保生命周期正确
    // 由于协程链是 A->B->C，我们需要确保 C 的生命周期覆盖整个链
    std::cout << "[Main] 创建协程 C..." << std::endl;
    auto task_c = coroutine_c();
    
    std::cout << "[Main] 创建协程 B..." << std::endl;
    auto task_b = coroutine_b(task_c);
    
    std::cout << "[Main] 创建协程 A..." << std::endl;
    auto task_a = coroutine_a(task_b);
    
    std::cout << std::endl;
    std::cout << "[Main] 启动协程链..." << std::endl;
    std::cout << "========================================" << std::endl;
    
    task_a.start();
    
    std::cout << "========================================" << std::endl;
    std::cout << "[Main] 程序结束" << std::endl;
    
    return 0;
}
