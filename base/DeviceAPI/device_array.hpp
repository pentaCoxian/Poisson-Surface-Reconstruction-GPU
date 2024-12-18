/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Author: Anatoly Baskeheev, Itseez Ltd, (myname.mysurname@mycompany.com)
 */

#ifndef DEVICE_ARRAY_HPP_
#define DEVICE_ARRAY_HPP_

#include "device_memory.hpp"

#include <vector>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \brief @b DeviceArrayPCL class 带有引用计数的GPU内存类型容器
  *
  * \note Typed container for GPU memory with reference counting.
  *
  * \author Anatoly Baksheev
  */
template<class T>
class DeviceArrayPCL : public DeviceMemory
{
    public:
        /** \brief Element type. */
        typedef T type;

        /** \brief Element size. */
        enum { elem_size = sizeof(T) };

        /** \brief Empty constructor. */
        DeviceArrayPCL();

        /** \brief 在GPU内存中分配内部缓冲区
          * \param size_t: 要分配的元素数量
          * */
        DeviceArrayPCL(size_t size);

        /** \brief 使用用户分配的缓冲区初始化。在这种情况下禁用引用计数.
          * \param ptr: 指向缓存的指针
          * \param size: 元素数量
          * */
        DeviceArrayPCL(T *ptr, size_t size);

        /** \brief 拷贝构造函数. 只是增加引用计数器. */
        DeviceArrayPCL(const DeviceArrayPCL& other);

        /** \brief 指定操作符. 只是增加引用计数器. */
        DeviceArrayPCL& operator = (const DeviceArrayPCL& other);

        /** \brief 在GPU内存中分配内部缓冲区。如果内部缓冲区是在函数用新的大小重新创建之前创建的。如果新旧大小相等，它什么也不做.
          * \param size: 元素数量
          * */
        void create(size_t size);

        /** \brief 如果需要，减少引用计数器并释放内部缓冲区. */
        void release();

        /** \brief 执行数据复制。如果目标大小不同，它将被重新分配.
          * \param other_arg: 拷贝的目标容器
          * */
        void copyTo(DeviceArrayPCL& other) const;

        /** \brief 上传数据到GPU内存的内部缓冲区。它在内部调用create()函数以确保内部缓冲区大小足够.
          * \param host_ptr_arg: host中需要上传到GPU的数据的指针
          * \param size: 元素个数
          * */
        void upload(const T *host_ptr, size_t size);

        /** \brief 从GPU下载缓存中的内容到CPU
          * \param host_ptr_arg: 指向下载后数据的缓存指针
          * */
        void download(T *host_ptr) const;

        /** \brief Uploads data to internal buffer in GPU memory. It calls create() inside to ensure that intenal buffer size is enough.
          * \param data: host vector to upload from
          * */
        template<class A>
        void upload(const std::vector<T, A>& data);

         /** \brief Downloads data from internal buffer to CPU memory
           * \param data:  host vector to download to
           * */
        template<typename A>
        void download(std::vector<T, A>& data) const;

        /** \brief Performs swap of data pointed with another device array.
          * \param other: device array to swap with
          * */
        void swap(DeviceArrayPCL& other_arg);

        /** \brief Returns pointer for internal buffer in GPU memory. */
        T* ptr();

        /** \brief Returns const pointer for internal buffer in GPU memory. */
        const T* ptr() const;

        //using DeviceMemory::ptr;

        /** \brief Returns pointer for internal buffer in GPU memory. */
        operator T*();

        /** \brief Returns const pointer for internal buffer in GPU memory. */
        operator const T*() const;

        /** \brief Returns size in elements. */
        size_t size() const;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \brief @b DeviceArray2DPCL class  带引用计数的倾斜GPU内存的类型化容器
  *
  * \note Typed container for pitched GPU memory with reference counting.
  *
  * \author Anatoly Baksheev
  */
template<class T>
class DeviceArray2DPCL : public DeviceMemory2D
{
    public:
        /** \brief Element type. */
        typedef T type;

        /** \brief Element size. */
        enum { elem_size = sizeof(T) };

        /** \brief Empty constructor. */
        DeviceArray2DPCL();

        /** \brief Allocates internal buffer in GPU memory
          * \param rows: number of rows to allocate
          * \param cols: number of elements in each row
          * */
        DeviceArray2DPCL(int rows, int cols);

         /** \brief Initializes with user allocated buffer. Reference counting is disabled in this case.
          * \param rows: number of rows
          * \param cols: number of elements in each row
          * \param data: pointer to buffer
          * \param stepBytes: stride between two consecutive rows in bytes
          * */
        DeviceArray2DPCL(int rows, int cols, void *data, size_t stepBytes);

        /** \brief Copy constructor. Just increments reference counter. */
        DeviceArray2DPCL(const DeviceArray2DPCL& other);

        /** \brief Assigment operator. Just increments reference counter. */
        DeviceArray2DPCL& operator = (const DeviceArray2DPCL& other);

        /** \brief Allocates internal buffer in GPU memory. If internal buffer was created before the function recreates it with new size. If new and old sizes are equal it does nothing.
           * \param rows: number of rows to allocate
           * \param cols: number of elements in each row
           * */
        void create(int rows, int cols);

        /** \brief Decrements reference counter and releases internal buffer if needed. */
        void release();

        /** \brief Performs data copying. If destination size differs it will be reallocated.
          * \param other: destination container
          * */
        void copyTo(DeviceArray2DPCL& other) const;

        /** \brief Uploads data to internal buffer in GPU memory. It calls create() inside to ensure that intenal buffer size is enough.
          * \param host_ptr: pointer to host buffer to upload
          * \param host_step: stride between two consecutive rows in bytes for host buffer
          * \param rows: number of rows to upload
          * \param cols: number of elements in each row
          * */
        void upload(const void *host_ptr, size_t host_step, int rows, int cols);

        /** \brief Downloads data from internal buffer to CPU memory. User is resposible for correct host buffer size.
          * \param host_ptr: pointer to host buffer to download
          * \param host_step: stride between two consecutive rows in bytes for host buffer
          * */
        void download(void *host_ptr, size_t host_step) const;

        /** \brief Performs swap of data pointed with another device array.
          * \param other: device array to swap with
          * */
        void swap(DeviceArray2DPCL& other_arg);

        /** \brief Uploads data to internal buffer in GPU memory. It calls create() inside to ensure that intenal buffer size is enough.
          * \param data: host vector to upload from
          * \param cols: stride in elements between  two consecutive rows in bytes for host buffer
          * */
        template<class A>
        void upload(const std::vector<T, A>& data, int cols);

        /** \brief Downloads data from internal buffer to CPU memory
           * \param data: host vector to download to
           * \param cols: Output stride in elements between two consecutive rows in bytes for host vector.
           * */
        template<class A>
        void download(std::vector<T, A>& data, int& cols) const;

        /** \brief Returns pointer to given row in internal buffer.
          * \param y_arg: row index
          * */
        T* ptr(int y = 0);

        /** \brief Returns const pointer to given row in internal buffer.
          * \param y_arg: row index
          * */
        const T* ptr(int y = 0) const;
        
        //using DeviceMemory2D::ptr;

        /** \brief Returns pointer for internal buffer in GPU memory. */
        operator T*();

        /** \brief Returns const pointer for internal buffer in GPU memory. */
        operator const T*() const;
        
        /** \brief Returns number of elements in each row. */
        int cols() const;

        /** \brief Returns number of rows. */
        int rows() const;

        /** \brief Returns step in elements. */
        size_t elem_step() const;
};

#include "device_array_impl.hpp"

#endif /* DEVICE_ARRAY_HPP_ */
