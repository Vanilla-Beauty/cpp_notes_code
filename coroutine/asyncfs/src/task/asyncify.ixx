export module asyncpp.task:asyncify;

export import :queue;
export import :loop;
export import :coroutine;

import asyncpp.core;

namespace asyncpp::task {
    using asyncpp::core::Invocable;

    // 默认的AsyncTaskSuspender（当任务函数返回类型不为void时）
    template <typename ResultType>
    void defaultAsyncAwaitableSuspend(
        Awaitable<ResultType>* awaitable,
        AsyncTaskResumer resumer,
        CoroutineHandle& h
    ) {
        auto& asyncTaskQueue = AsyncTaskQueue::getInstance();
        asyncTaskQueue.enqueue({
            .handler = [resumer, awaitable] {
                awaitable->_taskResult = awaitable->_taskHandler();
                resumer();
            }
        });
    }

    /* 默认的AsyncTaskSuspender（当任务函数返回类型为void时的特化版本）
     *
     * 当f的返回类型为void时，函数f没有返回值。因此，我们定义了一个函数返回类型为void的特化版本，
     * 在该版本中构造的AsyncTask对象的handler调用用户函数f后，直接调用resumer唤醒协程，
     * 不会将f的返回值存储到Awaitable对象中。
    */
    template <>
    void defaultAsyncAwaitableSuspend<void>(
        Awaitable<void>* awaitable,
        AsyncTaskResumer resumer,
        CoroutineHandle& h
    ) {
        auto& asyncTaskQueue = AsyncTaskQueue::getInstance();
        asyncTaskQueue.enqueue({
            .handler = [resumer, awaitable] {
                awaitable->_taskHandler();
                resumer();
            }
        });
    }

    // 异步化工具函数，支持将普通函数f异步化
    export template <Invocable T>
    auto asyncify(
        T taskHandler,
        AsyncTaskSuspender<std::invoke_result_t<T>> suspender =
            defaultAsyncAwaitableSuspend<std::invoke_result_t<T>>
    ) {
        return Awaitable<std::invoke_result_t<T>> {
            ._taskHandler = taskHandler,
                ._suspender = suspender
        };
    }
}
