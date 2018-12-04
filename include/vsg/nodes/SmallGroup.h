#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/core/ref_ptr.h>
#include <vsg/core/dynamic_array.h>

#include <vsg/nodes/Node.h>

#include <array>
#include <vector>

namespace vsg
{

    class VSG_DECLSPEC SmallGroup : public Inherit<Node, SmallGroup>
    {
    public:
        using Children = dynamic_array<ref_ptr<Node>, 4>;
        using size_type = Children::size_type;

        SmallGroup(size_type size = 0);
        SmallGroup(Allocator* allocator, size_type size = 0);

        template<class N, class V>
        static void t_traverse(N& node, V& visitor)
        {
            for(auto& child : node._children) child->accept(visitor);
        }

        void traverse(Visitor& visitor) override { t_traverse(*this, visitor); }
        void traverse(ConstVisitor& visitor) const override { t_traverse(*this, visitor); }
        void traverse(DispatchTraversal& visitor) const override { t_traverse(*this, visitor); }
        void traverse(CullTraversal& visitor) const override { t_traverse(*this, visitor); }

        void read(Input& input) override;
        void write(Output& output) const override;

        void resize(size_type size);

        void setChild(size_type pos, vsg::Node* node) { _children[pos] = node; }
        vsg::Node* getChild(size_type pos) { return _children[pos].get(); }
        const vsg::Node* getChild(size_type pos) const { return _children[pos].get(); }

        size_type getNumChildren() const noexcept { return _children.size(); }

        Children& getChildren() { return _children; }
        const Children& getChildren() const { return _children; }

    protected:
        virtual ~SmallGroup();

        Children _children;
    };

    VSG_type_name(vsg::SmallGroup);

} // namespace vsg
