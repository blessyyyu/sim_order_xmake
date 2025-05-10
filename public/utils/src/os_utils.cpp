#include "utils/os_utils.h"

#include "utils/exception_utils.h"

#ifdef _WINDOWS
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#else
#include <dirent.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif  // _WINDOWS

namespace library {
    namespace utils {
#if 1
        uintptr_t os::LoadMmapBuffer(const std::string& path, size_t size, bool lazy, bool readOnly) {
#ifdef _WINDOWS
            HANDLE dumpFileDescriptor = CreateFileA(
                path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (dumpFileDescriptor == INVALID_HANDLE_VALUE) {
                throw library::utils::Exception("failed to open file for page " + path);
            }

            HANDLE fileMappingObject = CreateFileMapping(dumpFileDescriptor, NULL, PAGE_READWRITE, 0, size, NULL);
            if (fileMappingObject == NULL) {
                int nRet = GetLastError();
                throw library::utils::Exception("unable to mmap for page " + path);
            }

            void* buffer = MapViewOfFile(fileMappingObject, FILE_MAP_ALL_ACCESS, 0, 0, size);

            if (buffer == nullptr) {
                int nRet = GetLastError();
                throw library::utils::Exception("failed to load page " + path + ", MapViewOfFile Error " + std::to_string(nRet));
            }
            CloseHandle(fileMappingObject);
            CloseHandle(dumpFileDescriptor);
#else
            // 创建映射文件
            int fd = open(path.c_str(), readOnly ? O_RDONLY : (O_RDWR | O_CREAT), readOnly ? (mode_t)0400 : (mode_t)0600);
            if (fd < 0) {
                throw library::utils::Exception("failed to open file for page " + path);
            }

            // 文件大小不足映射长度时，生成空洞文件（开启大页时，下面的写入操作会报错，直接忽略即可）
            if (!readOnly) {
                auto len = lseek(fd, 0, SEEK_END);
                if (len < size) {
                    lseek(fd, size - 1, SEEK_SET);
                    write(fd, "", 1);
                }
            }

            int flags = MAP_SHARED;
            if (!lazy) {
                // 进行页预加载
                flags = flags | MAP_POPULATE;
            }

            void* buffer = mmap(nullptr, size, readOnly ? PROT_READ : (PROT_READ | PROT_WRITE), flags, fd, 0);
            if (buffer == MAP_FAILED) {
                close(fd);
                throw library::utils::Exception("Error mapping file to buffer");
            }

            if (!lazy) {
                // lock内存，忽略失败的情况
                mlock(buffer, size);
            }

            close(fd);
#endif  // _WINDOWS

            return reinterpret_cast<uintptr_t>(buffer);
        }

        bool os::ReleaseMmapBuffer(uintptr_t address, size_t size, bool lazy) {
            void* buffer = reinterpret_cast<void*>(address);
#ifdef _WINDOWS
            FlushViewOfFile(buffer, 0);
            UnmapViewOfFile(buffer);
#else
            // unlock and unmap
            if (!lazy && munlock(buffer, size) != 0) {
                return false;
            }

            if (munmap(buffer, size) != 0) {
                return false;
            }
#endif  // _WINDOWS
            return true;
        }
#endif

#if 0
        uintptr_t os::LoadMmapBuffer(const std::string& path, size_t size, bool lazy)
        {
#ifdef _WINDOWS
#else
            key_t key = ftok(path.c_str(), 0x6666);
            if (key < 0)
            {
                throw library::utils::Exception("failed to ftok " + path);
            }

            int shmid = 0;
            if ((shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | SHM_HUGETLB | 0600)) < 0)
            {
                if (errno == EEXIST)
                {
                    if ((shmid = shmget(key, 0, 0)) < 0)
                    {
                        throw library::utils::Exception("failed to shmget " + path);
                    }
                }
                else
                {
                    throw library::utils::Exception("failed to shmget " + path);
                }
            }

            // 映射共享内存
            void* buffer = shmat(shmid, nullptr, 0);

            // 锁定内存，禁止换出
            if (!lazy && shmctl(shmid, SHM_LOCK, nullptr) != 0)
            {
                shmdt(buffer);
                shmctl(shmid, IPC_RMID, nullptr);
                throw library::utils::Exception("failed to lock memory for page " + path);
            }

#endif  // _WINDOWS

            return reinterpret_cast<uintptr_t>(buffer);
        }

        bool os::ReleaseMmapBuffer(uintptr_t address, size_t size, bool lazy)
        {
            void* buffer = reinterpret_cast<void*>(address);
#ifdef _WINDOWS
#else

            if (shmdt(buffer) != 0)
            {
                return false;
            }
#endif  // _WINDOWS
            return true;
        }
#endif

    }  // namespace utils
}  // namespace library
