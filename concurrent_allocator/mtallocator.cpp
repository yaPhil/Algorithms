#include<vector>
#include<map>
#include<set>
#include<mutex>
#include<thread>
#include<algorithm>

class Allocator {
public:
    Allocator() {
        globalHeap_ = new Heap(-1);
        heapsNumber_ = std::max(std::thread::hardware_concurrency() * 2,(unsigned int)8);
        for (int i = 0; i < heapsNumber_; i++) {
            heaps_.push_back(new Heap(i, globalHeap_));
        }
    }

    ~Allocator() {
        for (auto heap : heaps_) {
            delete heap;
        }
        delete globalHeap_;
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
            //MemoryController::getInstance().remember(ptr);
            return (void*)((char*)ptr + OFFSET);
        }
        int threadId = std::hash<std::thread::id>()(std::this_thread::get_id()) % heapsNumber_;
        return heaps_[threadId]->allocate(totalSize);
    }

    void deallocate(void* ptr) {
        if (ptr == nullptr) {
            return;
        }
        BlockOwner* info = (BlockOwner*)((char*)ptr - OFFSET);
        if (info->owner == nullptr) {
            std::free(info);
            //MemoryController::getInstance().forget(info);
            return;
        }
        Heap* ownerHeap = findOwner((SuperBlock*)info->owner);
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
    static const long double FULLNESS_LEVEL;

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
        void setHeapID(int id) {
            heapID_ = id;
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
        void updateInfo(int used) {
            usedMemory_ += used;
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
                for(int i = 0; i < (int)fullBlocks_.size(); ++i) {
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
        ~Basket() {
            for (auto block : superBlocks_) {
                delete block;
            }
            for (auto block : fullBlocks_) {
                delete block;
            }
        }
    private:
        size_t allocatedMemory_;
        size_t usedMemory_;
        size_t iterator = 0;
        std::vector<SuperBlock*> superBlocks_;
        std::vector<SuperBlock*> fullBlocks_;
    };

    struct BlockOwner {
        SuperBlock* owner;
    };

    static const size_t OFFSET = sizeof(BlockOwner);

    class Heap {
    public:
        Heap(int heapId, Heap *globalHeap = nullptr) {
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
            size_t binIndex = normolizeSize_(size);
            Basket& currentBasket = baskets_[binIndex];
            std::pair<SuperBlock*, void*> localBlock = currentBasket.getBlock();
            if (localBlock.first == nullptr) {
                std::unique_lock<std::mutex> lockGlobalHeap(globalHeap_->mutex_);
                Basket& globalBasket = globalHeap_->baskets_[binIndex];
                std::pair<SuperBlock*, void*> globalBlock = globalBasket.getBlock();
                if (globalBlock.first != nullptr) {
                    localBlock.first = globalBlock.first;
                    localBlock.second = globalBlock.second;
                    localBlock.first->setHeapID(id_);
                    globalBasket.updateMemory(-(int)localBlock.first->getUsedMemory(), -(int)SUPERBLOCK_SIZE);
                    currentBasket.updateMemory((int)localBlock.first->getUsedMemory(), (int)SUPERBLOCK_SIZE);
                }
                else {
                    size_t normedSize = getNormolizedSize_(size);
                    localBlock.first = new SuperBlock(normedSize);
                    localBlock.first->setHeapID(id_);
                    localBlock.second = localBlock.first->getBlock();
                    currentBasket.updateMemory(0, SUPERBLOCK_SIZE);
                }
            }
            currentBasket.addSuperBlock(localBlock.first);
            BlockOwner* owner = (BlockOwner*)localBlock.second;
            owner->owner = localBlock.first;
            return (void*)((char*)owner + OFFSET);
        }

        void deallocate(void* ptr, SuperBlock* superBlock) {
            size_t binSize = normolizeSize_(superBlock->getBlockSize());
            Basket& currentBin = baskets_[binSize];
            superBlock->updateInfo(-(int)superBlock->getBlockSize());
            currentBin.updateMemory(-(int)superBlock->getBlockSize(), 0);
            currentBin.deallocate(superBlock, ptr);
            if (id_ == -1) {
                mutex_.unlock();
                return;
            }
            returnEmptiest(currentBin, superBlock);
            this->mutex_.unlock();
        }
        std::mutex& getMutex() {
            return mutex_;
        }
    private:
        std::vector<Basket> baskets_;
        Heap* globalHeap_;
        int id_;
        std::mutex mutex_;
        size_t normolizeSize_(size_t size) {
            size_t index = 0;
            size_t normSize = BLOCK_SIZE;
            while (normSize < size) {
                normSize *= 2;
                index++;
            }
            return index;
        }
        size_t getNormolizedSize_(size_t size) {
            size_t index = 0;
            size_t normSize = BLOCK_SIZE;
            while (normSize < size) {
                normSize *= 2;
                index++;
            }
            return normSize;
        }

        void returnEmptiest(Basket &basket, SuperBlock *block) {
            if ((basket.getUsedMemory() < basket.getAllocatedMemory() - SUPERBLOCK_SIZE * EMPTY_BLOCK_NUMBER) &&
                    basket.getUsedMemory() < (1 - FULLNESS_LEVEL) * basket.getAllocatedMemory()) {
                std::unique_lock<std::mutex> lock(globalHeap_->mutex_);
                size_t binSize = normolizeSize_(block->getBlockSize());
                Basket& globalBasket = globalHeap_->baskets_[binSize];
                SuperBlock* emptiestBlock = basket.getSuperBlock();
                if (emptiestBlock != nullptr) {
                    emptiestBlock->setHeapID(-1);
                    globalBasket.updateMemory((int)emptiestBlock->getUsedMemory(), SUPERBLOCK_SIZE);
                    basket.updateMemory(-(int)emptiestBlock->getUsedMemory(), -(int)SUPERBLOCK_SIZE);
                    globalBasket.addSuperBlock(emptiestBlock);
                }
            }
        }
    };

    Heap* findOwner(SuperBlock* superBlock) {
        int current = -1;
        Heap* currentHeap = globalHeap_;
        bool isFirstIter = true;
        do {
            if (!isFirstIter) {
                if (current == -1) {
                    globalHeap_->getMutex().unlock();
                }
                else {
                    heaps_[current]->getMutex().unlock();
                }
            }
            current = superBlock->getHeapID();
            if (current == -1) {
                globalHeap_->getMutex().lock();
                currentHeap = globalHeap_;
            }
            else {
                heaps_[current]->getMutex().lock();
                currentHeap = heaps_[current];
            }
            isFirstIter = false;
        } while (current != superBlock->getHeapID());
        return currentHeap;
    }

    std::vector<Heap*> heaps_;
    Heap* globalHeap_;
    int heapsNumber_;
};
const long double Allocator::FULLNESS_LEVEL = 0.75;
extern void* mtalloc(size_t bytes) {
    return Allocator::getInstance().allocate(bytes);
}

extern void mtfree(void* ptr) {
    Allocator::getInstance().deallocate(ptr);
}