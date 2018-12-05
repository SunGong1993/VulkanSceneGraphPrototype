#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/core/Allocator.h>

namespace vsg
{

    template<typename T, std::size_t N>
    class dynamic_array
    {
    public:

        using value_type = T;
        using iterator = value_type*;
        using const_iterator = const value_type*;
        using size_type = std::uint32_t;

        dynamic_array() :
            _size(0),
            _ptr(_data)
        {}

        explicit dynamic_array(size_type size) : _size(size)
        {
            if (size>N)
            {
                _ptr = new T[size];
            }
            else
            {
                _ptr = _data;
            }
        }

        dynamic_array(Allocator* allocator, size_type size) : _size(size)
        {
            if (size>N)
            {
                if (allocator)
                {
                    _ptr = allocator->newArray<T>(size);
                }
                else
                {
                    _ptr = new T[size];
                }
            }
            else
            {
                _ptr = _data;
            }
        }

        ~dynamic_array()
        {
            if (_size>N) delete [] _ptr;
        }

        void clear(Allocator* allocator=nullptr)
        {
            if (_size>N)
            {
                if (allocator)
                {
                    allocator->deleteArray(_ptr, _size);
                }
                else
                {
                    delete [] _ptr;
                }
                _ptr = _data;
            }
            else destroy_range(_ptr, _ptr+_size);
            _size = 0;
        }

        bool small_size() const noexcept { return _size<=N; }

        size_type size() const noexcept { return _size; }

        void resize(size_type new_size)
        {
            if (_size==new_size) return;

            T* new_ptr = new_size>N ? new T[new_size] : _data;

            if (new_ptr != _ptr)
            {
                size_type min_size = std::min(new_size, _size);

                T* original_s = _ptr;
                T* original_e = _ptr+min_size;
                T* new_s = new_ptr;

                // need to look at utlizing std::move.
                for(;original_s<original_e; ++original_s, ++new_s)
                {
                    (*new_s) = (*original_s);
                }

                if (_size>N) delete [] _ptr;
                else destroy_range(_ptr, _ptr+_size);

                _ptr = new_ptr;
                _size = new_size;
            }
            else
            {
                // destroy any trailing that are no longer required
                if (new_size<_size)
                {
                    destroy_range(_ptr+new_size, _ptr+_size);
                }
                _size = new_size;
            }
        }

        void resize(Allocator* allocator, size_type new_size)
        {
            if (_size==new_size) return;

            T* new_ptr = new_size>N ? allocator->newArray<T>(new_size) : _data;

            if (new_ptr != _ptr)
            {
                size_type min_size = std::min(new_size, _size);

                T* original_s = _ptr;
                T* original_e = _ptr+min_size;
                T* new_s = new_ptr;

                // need to look at utlizing std::move.
                for(;original_s<original_e; ++original_s, ++new_s)
                {
                    (*new_s) = (*original_s);
                }

                if (_size>N) allocator->deleteArray(_ptr, _size);
                else destroy_range(_ptr, _ptr+_size);

                _ptr = new_ptr;
                _size = new_size;
            }
            else
            {
                // destroy any trailing that are no longer required
                if (new_size<_size)
                {
                    destroy_range(_ptr+new_size, _ptr+_size);
                }
            }
        }

        iterator begin() noexcept { return _ptr; }
        iterator end() noexcept { return _ptr+_size; }

        const_iterator begin() const noexcept { return _ptr; }
        const_iterator end() const noexcept { return _ptr+_size; }

        T& operator[] (size_t i) { return _ptr[i]; }
        const T& operator[] (size_t i) const { return _ptr[i]; }

        T& at(size_t i) { return _ptr[i]; }
        const T& at(size_t i) const { return _ptr[i]; }

    protected:

        void destroy_range(T* s, T* e)
        {
            for(; s<e; ++s)
            {
                s->~T();
            }
        }

        size_type _size;
        T* _ptr;
        T _data[N];
    };


} // namespace vsg
