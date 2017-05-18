#include<vector>
#include<map>
#include<set>
#include<mutex>
#include<thread>
#include<algorithm>

class Allocator {
public:
    Allocator() {
        mainHeap = new ThreadHeap(-1);
        for (unsigned int i = 0; i < HEAPS_NUMBER; i++) {
            heaps.push_back(new ThreadHeap(mainHeap,i));
        }
    }

    ~Allocator() {
        for (auto heap : heaps) {
            delete heap;
        }
        delete mainHeap;
    }

    void* allocate(size_t size) {
        size_t totalSize = size + OFFSET;
        if (totalSize > SUPERBLOCK_SIZE / 2) {
            void* ptr = std::malloc(totalSize);
            if (ptr == nullptr) {
                return nullptr;
            }
            BlockOwner* info = (BlockOwner*)ptr;
            info->owner = nullptr;
            MemoryController::getInstance().remember(ptr);
            return (void*)((char*)ptr + OFFSET);
        }
        return heaps[THREAD_ID]->allocate(totalSize);
    }

    void deallocate(void* ptr) {
        if (ptr == nullptr) {
            return;
        }
        BlockOwner* info = (information*)((char*)ptr - OFFSET);
        if (info->owner == nullptr) {
            std::free(info);
            MemoryController::getInstance().forget(info);
            return;
        }
        ThreadHeap* ownerHeap = findOwner((SuperBlock*)info->owner);
        ownerHeap->deallocate(info, (SuperBlock*)info->owner);
    }

    static Allocator& getInstance() {
        static Allocator instance;
        return instance;
    }

private:
    static const size_t SUPERBLOCK_SIZE = 1024 * 8;
    static const size_t BLOCK_SIZE = 8;
    static const size_t EMPTY_BLOCK_NUMBER = 5;
    static const long double FULLNESS_LEVEL = 0.75;

    static const size_t HEAPS_NUMBER =
            std::max(std::thread::hardware_concurrency() * 2,(unsigned int)8);
    thread_local const size_t THREAD_ID =
            std::hash<std::thread::id>()(std::this_thread::get_id()) % HEAPS_NUMBER;
    class SuperBlock {
    public:
        SuperBlock(size_t size) {
            begin_ = std::malloc(SUPERBLOCK_SIZE);
            usedMemory_ = 0;
            heapID_ = -1;
            blockSize_ = normalize(size);
            numberOfBlocks_ = SUPERBLOCK_SIZE / blockSize_;
            for (int i = 0; i < numberOfBlocks_; i++) {
                freeBlocks_.push_back(i * blockSize_);
            }
        }

        void* getBlock() {
//        if (currentPosition_ == numberOfBlocks_) {
//            return nullptr;
//        }
            size_t shift = freeBlocks_.back();
            freeBlocks_.pop_back();
            return (char*)begin_ + shift;
        }

        void freeBlock(void *block) {
            char* ptr = (char*)block;
            size_t position = ptr - (char*)begin_;
            freeBlocks_.push_back(position);
        }

        size_t getBlockSize() {
            return blockSize_;
        }
        int getHeapID() {
            return heapID_;
        }
        int getBlockNumber() {
            return numberOfBlocks_;
        }
        size_t getUsedMemory() {
            return usedMemory_;
        }
        bool isEmpty() {
            return freeBlocks_.empty();
        }
        ~SuperBlock() {
            std::free(begin_);
        }
    private:
        void* begin_;
        std::vector<size_t> freeBlocks_;
        size_t blockSize_;
        int heapID_;
        int numberOfBlocks_;
        size_t usedMemory_;

        std::mutex operationMutex;
        size_t normalize(size_t size) {
            size_t size2 = BLOCK_SIZE;
            while (size2 < size) {
                size2 *= 2;
            }
            return size2;
        }
    };

    class SuperblockOwner {
    public:
        ~SuperblockOwner() {
            for (auto block : blocks) {
                delete block;
            }
        }

        SuperBlock* getNewBlock(size_t blockSize) {
            SuperBlock* block = new SuperBlock(blockSize);
            blocks.push_back(block);
            return block;
        }

        static SuperblockOwner& getInstance() {
            static SuperblockOwner instance;
            return instance;
        }

    private:
        std::vector<SuperBlock*> blocks;
    };

    class Basket {
    public:
        Basket() {
            allocatedMemory_ = 0;
            usedMemory_ = 0;
        }
        void addSuperBlock(SuperBlock *superBlock) {
            if (superBlock->isEmpty()) {
                fullBlocks_.push_back(superBlock);
                return;
            }
            superBlocks_.push_back(superBlock);
        }
        SuperBlock* getSuperBlock() {
            if (superBlocks_.empty()) {
                return nullptr;
            }
            SuperBlock* block = superBlocks_.back();
            superBlocks_.pop_back();
            return block;
        }
        void deallocate(SuperBlock* superBlock, void *ptr) {
            if (superBlock->isEmpty()) {
                for(int i = 0; i < fullBlocks_.size(); ++i) {
                    if(fullBlocks_[i] == superBlock) {
                        std::swap(fullBlocks_[i], fullBlocks_.back());
                        break;
                    }
                }
                fullBlocks_.pop_back();
                superBlocks_.push_back(superBlock);
            }
            superBlock->freeBlock(ptr);
        }
        std::pair<SuperBlock*, void*> getBlock() {
            if (superBlocks_.empty()) {
                return std::make_pair((SuperBlock*)nullptr, (void*)nullptr);
            }
            SuperBlock* superBlock = superBlocks_.back();
            superBlocks_.pop_back();
            void* block = superBlock->getBlock();
            return std::make_pair(superBlock, block);
        }
        void updateMemory(int used, int allocated) {
                usedMemory_ += used;
                allocatedMemory_ += allocated;
        }
        size_t getUsedMemory() {
            return usedMemory_;
        }
        size_t getAllocatedMemory() {
            return allocatedMemory_;
        }
    private:
        size_t allocatedMemory_;
        size_t usedMemory_;
        size_t iterator = 0;
        std::vector<SuperBlock*> superBlocks_;
        std::vector<SuperBlock*> fullBlocks_;
    };

    struct BlockOwner {
        void* owner;
    };

    class ThreadHeap {
    public:
        ThreadHeap(ThreadHeap* globalHeap = nullptr, size_t heapId) {
            id_ = heapId;
            globalHeap_ = globalHeap;
            size_t curSize = 4;
            while (curSize <= SUPERBLOCK_SIZE / 2) {
                baskets_.push_back(Basket());
                curSize *= 2;
            }
        }

        void* allocate(size_t size) {
            std::unique_lock<std::mutex> lock(mutex_);
            size_t binIndex = normolizeSize(size);
            Basket& currentBasket = baskets_[binIndex];
            std::pair<SuperBlock*, void*> allocationInfo = currentBasket.getBlock();
            if (allocationInfo.first == nullptr) {
                std::unique_lock<std::mutex> lockMain(globalHeap_->mutex_);
                Basket& mainBin = globalHeap_->findBin(size);
                std::pair<SuperBlock*, void*> globalAllocationInfo = mainBin.getBlock();
                if (globalAllocationInfo.first != nullptr) {
                    allocationInfo.first = globalAllocationInfo.first;
                    allocationInfo.second = globalAllocationInfo.second;
                    allocationInfo.first->heapID_ = id_;
                    mainBin.updateMemory(allocationInfo.first->usedMemory_,SUPERBLOCK_SIZE, false);
                    currentBasket.updateMemory(allocationInfo.first->usedMemory_,SUPERBLOCK_SIZE, true);
                }
                else {
                    allocationInfo.first = SuperblockOwner::getInstance().getNewBlock(size);
                    allocationInfo.first->heapID_ = id_;
                    allocationInfo.second = allocationInfo.first->getBlock();
                    currentBasket.updateMemory(0, SUPERBLOCK_SIZE, true);
                }
            }
            currentBasket.addSuperBlock(allocationInfo.first);
            BlockOwner* info = (BlockOwner*)allocationInfo.second;
            info->owner = allocationInfo.first;
            return (void*)((char*)info + sizeof(BlockOwner));
        }

        void deallocate(void* ptr,SuperBlock* block) {
            Basket& currentBin = this->findBin(block->getBlockSize());
            block->usedMemory_ -= block->getBlockSize();
            currentBin.updateMemory(block->getBlockSize(), 0, false);
            currentBin.deallocate(block, ptr);
            if (this->id_ == -1) {
                this->mutex_.unlock();
                return;
            }
            tryReturnBlock(currentBin, block);
            this->mutex_.unlock();
        }

    private:
        std::vector<Basket> baskets_;
        ThreadHeap* globalHeap_;
        int id_;
        std::mutex mutex_;

        size_t normolizeSize(size_t size) {
            size_t index = 0;
            size_t normSize = BLOCK_SIZE;
            while (normSize < size) {
                normSize *= 2;
                index++;
            }
            return index;
        }

        void tryReturnBlock(Basket& bin,SuperBlock* block) {
            if ((bin.getUsedMemory() < bin.getAllocatedMemory() - SUPERBLOCK_SIZE * EMPTY_BLOCK_NUMBER) &&
                    bin.getUsedMemory() < (1 - FULLNESS_LEVEL) * bin.getAllocatedMemory()) {
                std::unique_lock<std::mutex> lock(globalHeap_->mutex_);
                Basket& mainBin = globalHeap_->findBin(block->getBlockSize());
                SuperBlock* emptiestBlock = bin.getSuperBlock();
                if (emptiestBlock != nullptr) {
                    emptiestBlock->heapID_ = -1;
                    mainBin.updateMemory(emptiestBlock->usedMemory_, SUPERBLOCK_SIZE, true);
                    bin.updateMemory(emptiestBlock->usedMemory_, SUPERBLOCK_SIZE, false);
                    mainBin.addSuperBlock(emptiestBlock);
                }
            }
        }
    };

    class MemoryController {
    public:

        static MemoryController& getInstance() {
            static MemoryController instance;
            return instance;
        }

        void remember(void* ptr) {
            pointers.insert(ptr);
        }

        void forget(void*  ptr) {
            pointers.erase(ptr);
        }

        ~MemoryController() {
            for (auto ptr : pointers) {
                std::free(ptr);
            }
        }

    private:
        std::set<void*> pointers;
    };

    const size_t OFFSET = sizeof(BlockOwner);

    std::vector<ThreadHeap*> heaps;
    ThreadHeap* mainHeap;

    ThreadHeap* findOwner(SuperBlock* block) {
        int current = -1;
        ThreadHeap* currentHeap = mainHeap;
        bool isFirstIter = true;
        do {
            if (!isFirstIter) {
                if (current == -1) {
                    mainHeap->mutex_.unlock();
                }
                else {
                    heaps[current]->mutex_.unlock();
                }
            }
            current = block->heapID_;
            if (current == -1) {
                mainHeap->mutex_.lock();
                currentHeap = mainHeap;
            }
            else {
                heaps[current]->mutex_.lock();
                currentHeap = heaps[current];
            }
            isFirstIter = false;
        } while (current != block->heapID_);
        return currentHeap;
    }
};

extern void* mtalloc(size_t bytes) {
    return Allocator::getInstance().allocate(bytes);
}

extern void mtfree(void* ptr) {
    Allocator::getInstance().deallocate(ptr);
}